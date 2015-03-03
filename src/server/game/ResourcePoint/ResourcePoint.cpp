#include "ResourcePoint.h"
#include "ResourcePointMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

ResourcePoint::ResourcePoint() { }
ResourcePoint::~ResourcePoint() { }

bool ResourcePoint::RemoveCreature(ObjectGuid guid)
{
    Creature* cr = HashMapHolder<Creature>::Find(guid);
    if (!cr)
        return false;

    uint32 dbGuid = cr->GetDBTableGUIDLow();
    cr->SetRespawnTime(0);
    cr->RemoveCorpse();
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN);
    stmt->setUInt32(0, guid);
    stmt->setUInt16(1, cr->GetMapId());
    stmt->setUInt32(2, 0);  // instance id, always 0 for world maps
    CharacterDatabase.Execute(stmt);

    cr->AddObjectToRemoveList();
    sObjectMgr->DeleteCreatureData(dbGuid);
    return true;
}

bool ResourcePoint::RemoveGameObject(ObjectGuid guid)
{
    GameObject* obj = HashMapHolder<GameObject>::Find(guid);
    if (!obj)
        return false;
    uint32 dbGuid = obj->GetDBTableGUIDLow();
    obj->SetRespawnTime(0);                                 // not save respawn time
    obj->Delete();
    sObjectMgr->DeleteGOData(dbGuid);
    return true;
}

void ResourcePoint::RemoveAllCreatures()
{
    for (std::vector<ObjectGuid>::iterator i = m_Creatures.begin(); i != m_Creatures.end(); ++i)
        RemoveCreature(*i);
    m_Creatures.clear();
}

void ResourcePoint::RemoveAllGameObjects()
{
    for (std::vector<ObjectGuid>::iterator i = m_GameObjects.begin(); i != m_GameObjects.end(); ++i)
        RemoveGameObject(*i);
    m_GameObjects.clear();
}

bool ResourcePoint::SpawnCreature(uint32 entry, uint32 mapId, float x, float y, float z, float o, TeamId teamId /* = TEAM_NEUTRAL */, uint32 spawntimedelay /* = 0 */, float spawndist /*= 0*/)
{
    if (uint32 guid = sObjectMgr->AddCreData(entry, mapId, x, y, z, o, spawntimedelay))
    {
        AddCreature(guid, entry);
        return true;
    }
    return false;
}

bool ResourcePoint::SpawnGameObject(uint32 entry, uint32 map, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3)
{
    if (uint32 guid = sObjectMgr->AddGOData(entry, map, x, y, z, o, 0, rotation0, rotation1, rotation2, rotation3))
    {
        AddGameObject(guid, entry);
        return true;
    }

    return false;
}

void ResourcePoint::AddCreature(uint32 guid, uint32 entry)
{
    if (!entry)
    {
        CreatureData const* data = sObjectMgr->GetCreatureData(guid);
        if (!data)
            return;
        entry = data->id;
    }

    m_Creatures.push_back(ObjectGuid(HIGHGUID_UNIT, entry, guid));
}

void ResourcePoint::AddGameObject(uint32 guid, uint32 entry)
{
    if (!entry)
    {
        GameObjectData const* data = sObjectMgr->GetGOData(guid);
        if (!data)
            return;
        entry = data->id;
    }

    m_GameObjects.push_back(ObjectGuid(HIGHGUID_GAMEOBJECT, entry, guid));
}

void ResourcePoint::HandlePlayerEnter(Player* player)
{
    if (HasPlayer(player))
        return;
    SendAllState(player);
    m_Players[player->GetTeamId()].insert(player->GetGUID());
}

void ResourcePoint::HandlePlayerLeave(Player* player)
{
    if (!player->GetSession()->PlayerLogout())
        ClearAllState(player);
    m_Players[player->GetTeamId()].erase(player->GetGUID());
}

void ResourcePoint::BroadcastMessage(int32 messageID)
{
    for (uint32 i = 0; i < BG_TEAMS_COUNT; ++i)
        for (GuidSet::iterator itr = m_Players[i].begin(); itr != m_Players[i].end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(*itr))
                player->SendDirectMessage(NULL);
}

void ResourcePoint::BroadcastZoneMessage(int32 messageID)
{
    if (m_Zone != 0)
        sWorld->SendZoneText(m_Zone, sObjectMgr->GetServerMessage(messageID).c_str());
}

void ResourcePoint::BoardcastWorldMessage(int32 messageID)
{
    sWorld->SendGlobalText(sObjectMgr->GetServerMessage(messageID).c_str(), NULL);
}

bool ResourcePoint::HasPlayer(Player* player)
{
    GuidSet::iterator itr = m_Players[player->GetTeamId()].find(player->GetGUID());
    return itr != m_Players[player->GetTeamId()].end();
}

void ResourcePoint::SendStateUpdate(uint32 index, uint32 value)
{
    for (uint32 i = 0; i < BG_TEAMS_COUNT; ++i)
        for (GuidSet::iterator itr = m_Players[i].begin(); itr != m_Players[i].end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(*itr))
                player->SendUpdateWorldState(index, value);
}

void ResourcePoint::ClearState(uint32 index)
{
    for (uint32 i = 0; i < BG_TEAMS_COUNT; ++i)
        for (GuidSet::iterator itr = m_Players[i].begin(); itr != m_Players[i].end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(*itr))
                player->SendUpdateWorldState(index, 0);
}

void ResourcePoint::Save()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_RESOURCE_POINT);
    stmt->setUInt32(0, m_Area);
    stmt->setUInt32(1, m_ControlledBy);
    stmt->setUInt32(2, m_GuardCount);
    stmt->setUInt32(3, m_WorkerCount);
    CharacterDatabase.Execute(stmt);
}

void ResourcePoint::SetGuardCount(uint32 count)
{
    if (count > m_MaxGuardCount)
        m_GuardCount = m_MaxGuardCount;
    else
        m_GuardCount = count;
}

void ResourcePoint::SetWorkerCount(uint32 count)
{
    if (count > m_MaxWorkerCount)
        m_WorkerCount = m_MaxWorkerCount;
    else
        m_WorkerCount = count;
}

void ResourcePoint::HandleKilledCreature(Unit* killer, Creature* victim)
{
    if (ControlledBy(RP_ALLIANCE) && victim->GetEntry() == m_AllianceWorkerEntry ||
        ControlledBy(RP_HORDE) && victim->GetEntry() == m_HordeWorkerEntry)
        DecreaseWorkerCount();
    else if (ControlledBy(RP_ALLIANCE) && victim->GetEntry() == m_AllianceGuardEntry ||
        ControlledBy(RP_HORDE) && victim->GetEntry() == m_HordeGuardEntry)
        DecreaseGuardCount();
}

void ResourcePoint::IncreaseGuardCount(bool broadcast /* = true */)
{
    if (m_GuardCount < m_MaxGuardCount)
    {
        ++m_GuardCount;
        if (broadcast)
            SendDefensePowerUpdate();
    }
}

void ResourcePoint::DecreaseGuardCount(bool broadcast /* = true */)
{
    if (m_GuardCount <= 1) // last guard, switch or reset controller faction.
    {
        //SwitchControlledFaction();
        return;
    }

    --m_GuardCount;
    if (broadcast)
        SendDefensePowerUpdate();
}

void ResourcePoint::IncreaseWorkerCount(bool broadcast /* = true */)
{
    if (m_WorkerCount < m_MaxWorkerCount)
    {
        ++m_WorkerCount;
        if (broadcast)
            SendProductionPowerUpdate();
    }
}

void ResourcePoint::DecreaseWorkerCount(bool broadcast /* = true */)
{
    if (m_WorkerCount <= 1) // last Worker.
    {
        return;
    }

    --m_WorkerCount;
    if (broadcast)
        SendProductionPowerUpdate();
}

bool ResourcePoint::ControlledBy(RPFaction faction)
{
    if (m_ControlledBy == faction)
        return true;
    return false;
}

uint32 ResourcePoint::Gather()
{
    if (m_CanGather)
    {
        uint32 resource = m_Resource;
        m_Resource = 0;
        return resource;
    }

    return 0;
}

void ResourcePoint::SendAllState(Player* player)
{
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_CONTROLLED_BY_HORDE, ControlledBy(RP_HORDE) ? 1 : 0);
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_CONTROLLED_BY_ALLIANCE, ControlledBy(RP_ALLIANCE) ? 1 : 0);
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_INFORCE_POWER, CurrentDefensePower());
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_PRODUCTION_POWER, CurrentProductionPower());
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_REINFORCE_POWER, CurrentReinforcePower());
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_MAX_INFORCE_POWER, CurrentMaxDefensePower());
    xResourcePointMgr->SendStateTo(player, WORLDSTATE_MAX_PRODUCTION_POWER, CurrentMaxProductionPower());
}

void ResourcePoint::ClearAllState(Player* player)
{
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_CONTROLLED_BY_HORDE);
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_CONTROLLED_BY_ALLIANCE);
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_INFORCE_POWER);
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_PRODUCTION_POWER);
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_REINFORCE_POWER);
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_MAX_PRODUCTION_POWER);
    xResourcePointMgr->ClearStateOf(player, WORLDSTATE_MAX_INFORCE_POWER);
}