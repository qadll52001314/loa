#ifndef RESOURCE_POINT_H_
#define RESOURCE_POINT_H_

#include "Player.h"
#include "World.h"

#define WORLDSTATE_CONTROLLED_BY_HORDE      5999
#define WORLDSTATE_CONTROLLED_BY_ALLIANCE   6000
#define WORLDSTATE_INFORCE_POWER            6001
#define WORLDSTATE_REINFORCE_POWER          6006
#define WORLDSTATE_PRODUCTION_POWER         6002
#define WORLDSTATE_MAX_INFORCE_POWER        6007
#define WORLDSTATE_MAX_PRODUCTION_POWER     6008
#define WORLDSTATE_MAX_REINFORCE_POWER      6009

enum RPFaction
{
    RP_ALLIANCE = 0,
    RP_HORDE    = 1,
    RP_NEUTURAL = 2
};

struct RPGameObject
{
    uint32 entry;
    uint32 map;
    float x;
    float y;
    float z;
    float o;
    float rot0;
    float rot1;
    float rot2;
    float rot3;
};

struct RPCreature
{
    uint32 entry;
    uint32 map;
    float x;
    float y;
    float z;
    float o;
};

class ResourcePoint
{
    friend class ResourcePointMgr;

public:
    ResourcePoint();
    virtual ~ResourcePoint();

    virtual void Update(uint32 diff) { }
    virtual void Create() = 0;

    bool SpawnGameObject(uint32 entry, uint32 map, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3);
    bool SpawnCreature(uint32 entry, uint32 map, float x, float y, float z, float o, TeamId teamId = TEAM_NEUTRAL, uint32 spawntimedelay = 0, float spawndist = 0);
    void AddGameObject(uint32 guid, uint32 entry);
    void AddCreature(uint32 guid, uint32 entry);
    bool RemoveGameObject(ObjectGuid guid);
    bool RemoveCreature(ObjectGuid guid);
    void RemoveAllCreatures();
    void RemoveAllGameObjects();

    void BroadcastMessage(int32 message);
    void BroadcastZoneMessage(int32 message);
    void BoardcastWorldMessage(int32 message);

    void SendStateUpdate(uint32 index, uint32 value);
    void ClearState(uint32 index);
    void SendAllState(Player*);
    void ClearAllState(Player*);

    //virtual void HandlePvPKill(Player* killer, Unit* killed) { }
    void HandlePlayerEnter(Player*);
    void HandlePlayerLeave(Player*);

    bool HasPlayer(Player*);

    void HandleKilledCreature(Unit* killer, Creature* victim);

    virtual uint32 CurrentProductionPower() { return m_WorkerCount; }
    virtual uint32 CurrentDefensePower() { return m_GuardCount; }
    virtual uint32 CurrentMaxProductionPower() { return m_MaxWorkerCount; }
    virtual uint32 CurrentMaxDefensePower() { return m_MaxGuardCount; }
    virtual uint32 CurrentReinforcePower() { return m_Reinforcement; }
    uint32 GetWorkerCount() { return m_WorkerCount; }
    uint32 GetGuardCount() { return m_GuardCount; }
    uint32 GetReinforcement() { return m_Reinforcement; }
    uint32 GetMaxWorkerCount() { return m_MaxWorkerCount; }
    uint32 GetMaxGuardCount() { return m_MaxGuardCount; }
    void SetGuardCount(uint32 count);
    void SetWorkerCount(uint32 count);
    void SetReinforcement(uint32 reinforce) { m_Reinforcement = reinforce; }
    void IncreaseGuardCount(bool broadcast = true);
    void DecreaseGuardCount(bool broadcast = true);
    void IncreaseWorkerCount(bool broadcast = true);
    void DecreaseWorkerCount(bool broadcast = true);
    virtual void SendDefensePowerUpdate() { }
    virtual void SendProductionPowerUpdate() { }
    void CalculateWorkers();
    void SetGuardEntry(uint32 entryAlliance, uint32 entryHorde) { m_AllianceGuardEntry = entryAlliance; m_HordeGuardEntry = entryHorde; }
    void SetWorkerEntry(uint32 entryAlliance, uint32 entryHorde) { m_AllianceWorkerEntry = entryAlliance; m_HordeWorkerEntry = entryHorde; }
    void SetBaseStats(uint32 maxGuard, uint32 maxWorker)
    {
        m_MaxGuardCount = maxGuard;
        m_MaxWorkerCount = maxWorker;
    }
    virtual void Reset() = 0;

    bool ControlledBy(RPFaction faction);
    void SetControlledBy(RPFaction faction) { m_ControlledBy = faction; }

    void SetParentZone(uint32 zone) { m_Zone = zone; }
    uint32 GetRegisteredArea() { return m_Area; }
    void RegisterArea(uint32 area) { m_Area = area; }

    void Save();

    uint32 Gather();
private:
    GuidSet m_Players[2];
    std::vector<ObjectGuid> m_GameObjects;
    std::vector<ObjectGuid> m_Creatures;
    RPFaction m_ControlledBy;
    uint32 m_Zone; // parent zone, used for broadcast in a larger area.
    uint32 m_Area;
    uint32 m_AllianceWorkerEntry;
    uint32 m_HordeWorkerEntry;
    uint32 m_AllianceGuardEntry;
    uint32 m_HordeGuardEntry;
    uint32 m_WorkerCount;
    uint32 m_GuardCount;
    uint32 m_MaxWorkerCount;
    uint32 m_MaxGuardCount;
    uint32 m_Reinforcement;
    uint32 m_MaxReinforcement;
    uint32 m_Resource;
    bool m_CanGather;
};

#endif
