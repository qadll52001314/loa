#include "CapitalCity.h"
#include "CapitalCityMgr.h"
#include "World.h"

void CapitalCity::Save()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CAPITAL_CITY);
    stmt->setUInt32(0, m_Zone);
    stmt->setUInt32(1, m_Resource);
	stmt->setUInt32(2, m_MagicPower);
    stmt->setUInt32(3, m_Level);
    stmt->setUInt32(4, m_UpgradeResource);
    stmt->setUInt32(5, m_UpgradeMagicPower);
    CharacterDatabase.Execute(stmt);
}

void CapitalCity::HandlePlayerEnter(Player* player)
{
    SendAllState(player);
}

void CapitalCity::HandlePlayerLeave(Player* player)
{
    ClearAllState(player);
}

void CapitalCity::SendAllState(Player* player)
{
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_IN_CAPITAL_CITY, 1);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADING, 1);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_LEVEL, m_Level);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_RESOURCE, m_Resource);
	xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_MAGICPOWER, m_MagicPower);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS, m_UpgradeResource * 100 / xCapitalCityMgr->ResourceToNextLevel(m_Level));
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS, m_UpgradeMagicPower * 100 / xCapitalCityMgr->MagicPowerToNextLevel(m_Level));
}

void CapitalCity::ClearAllState(Player* player)
{
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_IN_CAPITAL_CITY);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADING);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_LEVEL);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_RESOURCE);
	xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_MAGICPOWER);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS);
}

void CapitalCity::SendStateUpdate()
{
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_LEVEL, m_Level);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_RESOURCE, m_Resource);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_MAGICPOWER, m_MagicPower);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS, m_UpgradeResource * 100 / xCapitalCityMgr->ResourceToNextLevel(m_Level));
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS, m_UpgradeMagicPower * 100 / xCapitalCityMgr->MagicPowerToNextLevel(m_Level));
}

std::string CapitalCity::GetName()
{
    AreaTableEntry const* zoneEntry = GetAreaEntryByAreaID(m_Zone);
    if (!zoneEntry) return "<error>";
    return zoneEntry->area_name[4];
}

void CapitalCity::AddResource(int32 resource, Player* player/* = NULL*/)
{
    if (!resource) return;
    m_Resource += resource;
    SendStateUpdate();
    if (!player)
        sWorld->SendGlobalChatMessageToFaction(sObjectMgr->GetServerMessage(14, GetName().c_str(), resource).c_str(), m_Faction);
    else
        sWorld->SendGlobalChatMessageToFaction(sObjectMgr->GetServerMessage(15, player->GetName().c_str(), GetName().c_str(), resource).c_str(), m_Faction);
}

void CapitalCity::AddMagicPower(int32 magic, Player* player/* = NULL*/)
{
    if (!magic) return;
    m_MagicPower += magic;
    SendStateUpdate();
    if (!player)
        sWorld->SendGlobalChatMessageToFaction(sObjectMgr->GetServerMessage(16, GetName().c_str(), magic).c_str(), m_Faction);
    else
        sWorld->SendGlobalChatMessageToFaction(sObjectMgr->GetServerMessage(17, player->GetName().c_str(), GetName().c_str(), magic).c_str(), m_Faction);
}

void CapitalCity::LevelUp(bool sendUpdate)
{
    m_UpgradeResource = 0;
    m_UpgradeMagicPower = 0;
    m_Level += 1;
    sWorld->SendGlobalChatMessageToFaction(sObjectMgr->GetServerMessage(18, GetName().c_str()).c_str(), m_Faction);
    if (sendUpdate)
        SendStateUpdate();
}

void CapitalCity::Update()
{
    if (m_Resource > 0 || m_MagicPower > 0)
    {
        uint32 resourceToNextLevel = xCapitalCityMgr->ResourceToNextLevel(m_Level);
        uint32 magicPowerToNextLevel = xCapitalCityMgr->MagicPowerToNextLevel(m_Level);
        if (!resourceToNextLevel || !magicPowerToNextLevel) // seems no entry for this level, need fix
        {
            TC_LOG_ERROR("legacy.capitalcity", "No upgrade info for capital city level %u, need fix!", m_Level);
            return;
        }
        uint32 resourceForThisRound = std::min(resourceToNextLevel / 20, m_Resource);
        resourceForThisRound = std::min(resourceForThisRound, resourceToNextLevel - m_UpgradeResource);
        uint32 magicPowerForThisRound = std::min(magicPowerToNextLevel / 20, m_MagicPower);
        magicPowerForThisRound = std::min(magicPowerForThisRound, magicPowerToNextLevel - m_UpgradeMagicPower);

        m_Resource -= resourceForThisRound;
        m_MagicPower -= magicPowerForThisRound;

        m_UpgradeResource += resourceForThisRound;
        m_UpgradeMagicPower += magicPowerForThisRound;

        if (m_UpgradeResource >= resourceToNextLevel && m_UpgradeMagicPower >= magicPowerToNextLevel) // update case
            LevelUp(false);
    }
    SendStateUpdate();
}