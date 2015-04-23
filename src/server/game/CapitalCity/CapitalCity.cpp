#include "CapitalCity.h"
#include "CapitalCityMgr.h"
#include "World.h"
#include "Chat.h"

void CapitalCity::Save()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CAPITAL_CITY);
    stmt->setUInt32(0, m_ID);
    stmt->setUInt32(1, m_Resource);
	stmt->setUInt32(2, m_MagicPower);
    stmt->setUInt32(3, m_Rank);
    stmt->setUInt32(4, m_UpgradeResource);
    stmt->setUInt32(5, m_UpgradeMagicPower);
    stmt->setBool(6, m_Upgrading);
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
    if (m_Upgrading)
        xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADING, 1);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_LEVEL, m_Rank);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_RESOURCE, m_Resource);
	xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_MAGICPOWER, m_MagicPower);
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS, m_UpgradeResource * 100 / xCapitalCityMgr->ResourceToNextLevel(m_Rank));
    //xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS, m_UpgradeMagicPower * 100 / xCapitalCityMgr->MagicPowerToNextLevel(m_Rank));
}

void CapitalCity::ClearAllState(Player* player)
{
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_IN_CAPITAL_CITY);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADING);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_LEVEL);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_RESOURCE);
	xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_MAGICPOWER);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS);
    //xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS);
}

void CapitalCity::SendStateUpdate()
{
    if (m_Upgrading)
        sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADING, 1);
    else
        sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADING, 0);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_LEVEL, m_Rank);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_RESOURCE, m_Resource);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_MAGICPOWER, m_MagicPower);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS, m_UpgradeResource * 100 / xCapitalCityMgr->ResourceToNextLevel(m_Rank));
    //sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS, m_UpgradeMagicPower * 100 / xCapitalCityMgr->MagicPowerToNextLevel(m_Rank));
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
        sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(14, GetName().c_str(), resource).c_str(), m_Team);
    else
        sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(15, player->GetName().c_str(), GetName().c_str(), resource).c_str(), m_Team);
    Save();
}

void CapitalCity::AddMagicPower(int32 magic, Player* player/* = NULL*/)
{
    if (!magic) return;
    m_MagicPower += magic;
    SendStateUpdate();
    if (!player)
        sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(16, GetName().c_str(), magic).c_str(), m_Team);
    else
        sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(17, player->GetName().c_str(), GetName().c_str(), magic).c_str(), m_Team);
    Save();
}

void CapitalCity::RankUp(bool sendUpdate)
{
    m_UpgradeResource = 0;
    m_UpgradeMagicPower = 0;
    m_Upgrading = false;
    m_Rank += 1;
    sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(18, GetName().c_str()).c_str(), m_Team);
    LearnRankUpSpells(m_Rank);
    if (sendUpdate)
        SendStateUpdate();
    Save();
}

void CapitalCity::Update()
{
    if (!m_Upgrading)
        TryStartUpgrade();
    else if (m_Resource > 0 || m_MagicPower > 0)
    {
        uint32 resourceToNextLevel = xCapitalCityMgr->ResourceToNextLevel(m_Rank);
        uint32 magicPowerToNextLevel = xCapitalCityMgr->MagicPowerToNextLevel(m_Rank);
        if (!resourceToNextLevel || !magicPowerToNextLevel) // seems no entry for this level, need fix
        {
            TC_LOG_ERROR("legacy.capitalcity", "No upgrade info for capital city level %u, need fix!", m_Rank);
            return;
        }
        uint32 resourceForThisRound = std::min((uint32)100, m_Resource);
        resourceForThisRound = std::min(resourceForThisRound, resourceToNextLevel - m_UpgradeResource);
        uint32 magicPowerForThisRound = std::min((uint32)100, m_MagicPower);
        magicPowerForThisRound = std::min(magicPowerForThisRound, magicPowerToNextLevel - m_UpgradeMagicPower);

        m_Resource -= resourceForThisRound;
        m_MagicPower -= magicPowerForThisRound;

        m_UpgradeResource += resourceForThisRound;
        m_UpgradeMagicPower += magicPowerForThisRound;

        if (m_UpgradeResource >= resourceToNextLevel && m_UpgradeMagicPower >= magicPowerToNextLevel) // update case
            RankUp(false);

        SendStateUpdate();
    }

    Save();
}

void CapitalCity::TryStartUpgrade(Player* commander)
{
    if (m_Upgrading)
        return;

    uint32 startResource = xCapitalCityMgr->StartResourceToNextLevel(m_Rank);
    uint32 startMagicPower = xCapitalCityMgr->StartMagicPowerToNextLevel(m_Rank);
    if (m_Resource < startResource || m_MagicPower < startMagicPower)
    {
        if (commander)
            ChatHandler(commander->GetSession()).PSendSysMessage(sObjectMgr->GetServerMessage(30, GetName().c_str(), startResource, startMagicPower).c_str());
        return;
    }

    m_Resource -= startResource;
    m_MagicPower -= startMagicPower;

    m_Upgrading = true;
    if (commander)
        sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(28, commander->GetName().c_str(), GetName().c_str(), m_Rank, m_Rank + 1).c_str(), m_Team);
    SendStateUpdate();
}

void CapitalCity::UpgradeStop(Player* commander)
{
    if (!m_Upgrading)
        return;

    m_Upgrading = false;
    if (commander)
        sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(29, commander->GetName().c_str(), GetName().c_str()).c_str(), m_Team);
    SendStateUpdate();

    Save();
}

void CapitalCity::LearnRankUpSpells(uint32 rank)
{
    CapitalCityResearchList list = xCapitalCityMgr->GetSpellsForRank(GetID(), rank);
    if (list.empty())
        return;

    for (int i = 0; i != list.size(); ++i)
        sWorld->AddGlobalSpellsToTeam(list[i], m_Team);
}
