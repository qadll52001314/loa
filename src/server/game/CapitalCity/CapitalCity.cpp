#include "CapitalCity.h"
#include "CapitalCityMgr.h"
#include "World.h"
#include "Chat.h"

void CapitalCity::Save()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CAPITAL_CITY);
    stmt->setUInt32(0, m_ID);
    stmt->setUInt32(1, m_Resource);
    stmt->setUInt32(2, m_Rank);
    stmt->setBool(3, m_Upgrading);
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
    xCapitalCityMgr->SendStateTo(player, WORLDSTATE_CC_UPGRADE_RESOURCE, xCapitalCityMgr->ResourceToNextLevel(m_Rank));
}

void CapitalCity::ClearAllState(Player* player)
{
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_IN_CAPITAL_CITY);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADING);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_LEVEL);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CAPITAL_CITY_RESOURCE);
    xCapitalCityMgr->ClearStateOf(player, WORLDSTATE_CC_UPGRADE_RESOURCE);
}

void CapitalCity::SendStateUpdate()
{
    if (m_Upgrading)
        sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADING, 1);
    else
        sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADING, 0);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_LEVEL, m_Rank);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CAPITAL_CITY_RESOURCE, m_Resource);
    sWorld->UpdateWorldStateInZone(m_Zone, WORLDSTATE_CC_UPGRADE_RESOURCE, xCapitalCityMgr->ResourceToNextLevel(m_Rank));
}

std::string CapitalCity::GetName()
{
    AreaTableEntry const* zoneEntry = GetAreaEntryByAreaID(m_Zone);
    if (!zoneEntry) return "<error>";
    return zoneEntry->area_name[sObjectMgr->GetDBCLocaleIndex()];
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

    if (m_Resource >= xCapitalCityMgr->ResourceToNextLevel(m_Rank))
        RankUp(true);
    else
        Save();
}

void CapitalCity::RankUp(bool sendUpdate)
{
    m_Resource -= xCapitalCityMgr->ResourceToNextLevel(m_Rank);
    m_Rank += 1;
    sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(18, GetName().c_str()).c_str(), m_Team);
    LearnRankUpSpells(m_Rank);
    if (sendUpdate)
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
