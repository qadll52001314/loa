#include "ResourcePointMgr.h"
#include "ScriptMgr.h"

ResourcePointMgr::ResourcePointMgr() { }
ResourcePointMgr::~ResourcePointMgr() { }

void ResourcePointMgr::HandlePlayerEnter(Player* player, uint32 area)
{
    ResourcePointMap::iterator itr = m_ResourcePoints.find(area);
    if (itr == m_ResourcePoints.end())
        return;

    itr->second->HandlePlayerEnter(player);
}

void ResourcePointMgr::HandlePlayerLeave(Player* player, uint32 area)
{
    ResourcePointMap::iterator itr = m_ResourcePoints.find(area);
    if (itr == m_ResourcePoints.end())
        return;

    itr->second->HandlePlayerLeave(player);
}

ResourcePoint* ResourcePointMgr::GetResourcePoint(uint32 area)
{
    ResourcePointMap::iterator itr = m_ResourcePoints.find(area);
    if (itr == m_ResourcePoints.end())
        return NULL;
    return itr->second;
}

void ResourcePointMgr::Update(uint32 diff)
{
    m_UpdateTimer += diff;
    if (m_UpdateTimer > RP_UPDATE_INVERVAL)
    {
        for (ResourcePointMap::iterator itr = m_ResourcePoints.begin(); itr != m_ResourcePoints.end(); ++itr)
            (itr->second)->Update(m_UpdateTimer);
        m_UpdateTimer = 0;
    }
}

void ResourcePointMgr::LoadResourcePoints()
{
    m_ResourcePoints.clear();

    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT area, scriptname, max_guard, max_worker FROM resource_points");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Resource Point definitions. DB table `resource_points` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 scriptId = sObjectMgr->GetScriptId(fields[1].GetCString());
        ResourcePoint* point = sScriptMgr->CreateResourcePoint(scriptId);
        point->SetBaseStats(fields[2].GetUInt32(), fields[3].GetUInt32());
        m_ResourcePoints[fields[0].GetUInt32()] = point;
        ++count;
    } while (result->NextRow());

    result = CharacterDatabase.Query("SELECT area, controlBy, guardCount, workerCount FROM resource_point_state");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            if (ResourcePoint* point = m_ResourcePoints[fields[0].GetUInt32()])
            {
                point->SetControlledBy((RPFaction)fields[1].GetUInt32());
                point->SetGuardCount(fields[2].GetUInt32());
                point->SetWorkerCount(fields[3].GetUInt32());
            }
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Resource Point definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    SetupResourcePoints();
}

void ResourcePointMgr::SetupResourcePoints()
{
    for (ResourcePointMap::iterator itr = m_ResourcePoints.begin(); itr != m_ResourcePoints.end(); ++itr)
        (itr->second)->Create();
}

void ResourcePointMgr::RegisterResourcePoint(uint32 area, ResourcePoint* point)
{
    m_ResourcePoints[area] = point;
}

void ResourcePointMgr::Save()
{
    for (ResourcePointMap::iterator itr = m_ResourcePoints.begin(); itr != m_ResourcePoints.end(); ++itr)
    {
        if (itr->second)
        {
            itr->second->Save();
            delete itr->second;
        }
    }
}

void ResourcePointMgr::HandleKilledCreature(uint32 area, Unit* killer, Creature* victim)
{
    if (ResourcePoint* point = GetResourcePoint(area))
        point->HandleKilledCreature(killer, victim);
}

void ResourcePointMgr::SendStateTo(Player* player, uint32 index, uint32 value)
{
    player->SendUpdateWorldState(index, value);
}

void ResourcePointMgr::ClearStateOf(Player* player, uint32 index)
{
    player->SendUpdateWorldState(index, 0);
}
