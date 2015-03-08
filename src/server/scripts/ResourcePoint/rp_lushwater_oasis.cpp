#include "rp_lushwater_oasis.h"

ResourcePoint_LushwaterOasis::ResourcePoint_LushwaterOasis() 
{
    _workerUpdateTimer = 0;
    _guardUpdateTimer = 0;
    SetWorkerEntry(0, 0);
    SetGuardEntry(ALLIANCE_GUARD_ENTRY, HORDE_GUARD_ENTRY);
}

void ResourcePoint_LushwaterOasis::Create()
{
    SetParentZone(17);
    SetWorkerCount(0);
    SetGuardCount(0);
    SetReinforcement(200);
    TakenControlBy(RP_HORDE);
}

void ResourcePoint_LushwaterOasis::Update(uint32 diff)
{
    UpdateGuard(diff);
    UpdateWorker(diff);
}

void ResourcePoint_LushwaterOasis::UpdateWorker(uint32 diff)
{
    if (GetWorkerCount() >= GetMaxWorkerCount())
        return;

    _workerUpdateTimer += diff;

    if (_workerUpdateTimer > WORKER_UPDATE_DIFF)
    {
        IncreaseWorkerCount();
        _workerUpdateTimer = 0;
    }
}

void ResourcePoint_LushwaterOasis::UpdateGuard(uint32 diff)
{
    if (GetGuardCount() >= GetMaxGuardCount())
        return;

    _guardUpdateTimer += diff;

    if (_guardUpdateTimer > GUARD_UPDATE_DIFF)
    {
        IncreaseGuardCount();
        _guardUpdateTimer = 0;
    }
}

void ResourcePoint_LushwaterOasis::SpawnAllianceUnits()
{
    for (int i = 0; i != LUSHWATER_FACTION_FLAG_COUNT; ++i)
    {
        SpawnGameObject(AllianceFlags[i].entry, AllianceFlags[i].map, AllianceFlags[i].x, AllianceFlags[i].y, AllianceFlags[i].z, AllianceFlags[i].o, AllianceFlags[i].rot0, AllianceFlags[i].rot1, AllianceFlags[i].rot2, AllianceFlags[i].rot3);
    }

    for (int i = 0; i != LUSHWATER_FACTION_GUARD_COUNT; ++i)
    {
        SpawnCreature(AllianceGuards[i].entry, AllianceGuards[i].map, AllianceGuards[i].x, AllianceGuards[i].y, AllianceGuards[i].z, AllianceGuards[i].o, TEAM_ALLIANCE, 0, 10.0f);
    }
}

void ResourcePoint_LushwaterOasis::SpawnHordeUnits()
{
    for (int i = 0; i != LUSHWATER_FACTION_FLAG_COUNT; ++i)
    {
        SpawnGameObject(HordeFlags[i].entry, HordeFlags[i].map, HordeFlags[i].x, HordeFlags[i].y, HordeFlags[i].z, HordeFlags[i].o, HordeFlags[i].rot0, HordeFlags[i].rot1, HordeFlags[i].rot2, HordeFlags[i].rot3);
    }

    for (int i = 0; i != LUSHWATER_FACTION_GUARD_COUNT; ++i)
    {
        SpawnCreature(HordeGuards[i].entry, HordeGuards[i].map, HordeGuards[i].x, HordeGuards[i].y, HordeGuards[i].z, HordeGuards[i].o, TEAM_HORDE, 0, 10.f);
    }
}

void ResourcePoint_LushwaterOasis::TakenControlBy(RPFaction faction)
{
    RemoveAllCreatures();
    RemoveAllGameObjects();
    Reset();
    switch (faction)
    {
        case RP_ALLIANCE:
            BroadcastZoneMessage(3);
            SpawnAllianceUnits();
            SetControlledBy(RP_ALLIANCE);
            SendStateUpdate(WORLDSTATE_CONTROLLED_BY_HORDE, 0);
            SendStateUpdate(WORLDSTATE_CONTROLLED_BY_ALLIANCE, 1);
            break;
        case RP_HORDE:
            BroadcastZoneMessage(2);
            SpawnHordeUnits();
            SetControlledBy(RP_HORDE);
            SendStateUpdate(WORLDSTATE_CONTROLLED_BY_HORDE, 1);
            SendStateUpdate(WORLDSTATE_CONTROLLED_BY_ALLIANCE, 0);
            break;
        case RP_NEUTURAL:
            break;
        default:
            break;
    }
}

void ResourcePoint_LushwaterOasis::SendDefensePowerUpdate()
{
    SendStateUpdate(WORLDSTATE_INFORCE_POWER, CurrentDefensePower());
    SendStateUpdate(WORLDSTATE_MAX_INFORCE_POWER, CurrentMaxDefensePower());
}

void ResourcePoint_LushwaterOasis::SendProductionPowerUpdate()
{
    SendStateUpdate(WORLDSTATE_PRODUCTION_POWER, CurrentProductionPower());
    SendStateUpdate(WORLDSTATE_MAX_PRODUCTION_POWER, CurrentMaxProductionPower());
}

uint32 ResourcePoint_LushwaterOasis::CurrentProductionPower()
{
    return GetWorkerCount();
}

uint32 ResourcePoint_LushwaterOasis::CurrentDefensePower()
{
    return GetGuardCount();
}

uint32 ResourcePoint_LushwaterOasis::CurrentMaxProductionPower()
{
    return GetMaxWorkerCount();
}

uint32 ResourcePoint_LushwaterOasis::CurrentMaxDefensePower()
{
    return GetMaxGuardCount();
}

uint32 ResourcePoint_LushwaterOasis::CurrentReinforcePower()
{
    return GetReinforcement();
}

void ResourcePoint_LushwaterOasis::Reset()
{
    SetWorkerCount(10);
    SetGuardCount(5);
}

class LushwaterOasis_ResourcePointScript : public ResourcePointScript
{
public:
    LushwaterOasis_ResourcePointScript() : ResourcePointScript("rp_lushwater_oasis") { }

    ResourcePoint* GetResourcePoint() const override
    {
        return new ResourcePoint_LushwaterOasis();
    }
};

void AddSC_rp_lushwater_oasis()
{
    new LushwaterOasis_ResourcePointScript();
}
