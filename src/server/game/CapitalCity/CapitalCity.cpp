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
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_LEVEL, GetLevel());
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_RESOURCE, GetResource());
	xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CAPITAL_CITY_MAGICPOWER, GetMagicPower());
}

void CapitalCity::ClearAllState(Player* player)
{
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_IN_CAPITAL_CITY);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_LEVEL);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_RESOURCE);
	xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_MAGICPOWER);
}

void CapitalCity::SendStateUpdate()
{
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_LEVEL, GetLevel());
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_RESOURCE, GetResource());
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_MAGICPOWER, GetMagicPower());
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

void CapitalCity::LevelUp()
{
    m_Level += 1;
    SendStateUpdate();
    sWorld->SendGlobalChatMessageToFaction(sObjectMgr->GetServerMessage(18, GetName().c_str()).c_str(), m_Faction);
}