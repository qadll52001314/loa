#include "ResourcePoint.h"

#define LUSHWATER_FACTION_FLAG_COUNT        12
#define LUSHWATER_FACTION_GUARD_COUNT       24
#define WORKER_UPDATE_DIFF                  5000
#define GUARD_UPDATE_DIFF                   8000
#define ALLIANCE_GUARD_ENTRY                43307
#define HORDE_GUARD_ENTRY                   43308

const RPGameObject AllianceFlags[LUSHWATER_FACTION_FLAG_COUNT] =
{
    { 192274, 1, -967.521f, -2165.91f, 127.953f, 1.72846f, 0, 0, 0.760596f, 0.649226f },
    { 192274, 1, -998.88f, -2228.72f, 92.4484f, 2.26929f, 0, 0, 0.906383f, 0.422456f },
    { 192274, 1, -1035.44f, -2249.63f, 102.171f, 5.29237f, 0, 0, 0.47539f, -0.879775f },
    { 192274, 1, -1096.02f, -2298.59f, 90.5079f, 4.70441f, 0, 0, 0.709921f, -0.704281f },
    { 192274, 1, -1164.81f, -2255.38f, 89.4484f, 3.76848f, 0, 0, 0.951277f, -0.308337f },
    { 192274, 1, -1189.35f, -2119.76f, 117.039f, 3.27892f, 0, 0, 0.997644f, -0.0686083f },
    { 192274, 1, -1132.53f, -2043, 86.9566f, 1.40705f, 0, 0, 0.646911f, 0.762565f },
    { 192274, 1, -1105.51f, -1981.77f, 88.7153f, 1.97101f, 0, 0, 0.833552f, 0.552441f },
    { 192274, 1, -1028.7f, -1969.9f, 112.327f, 1.38066f, 0, 0, 0.63679f, 0.771037f },
    { 192274, 1, -957.975f, -1998.67f, 88.7911f, 1.39462f, 0, 0, 0.642159f, 0.766572f },
    { 192274, 1, -903.871f, -1990.67f, 107.179f, 1.21464f, 0, 0, 0.570667f, 0.821182f },
    { 192274, 1, -960.855f, -2081.44f, 81.2855f, 1.73823f, 0, 0, 0.763759f, 0.645501f }
};

const RPGameObject HordeFlags[LUSHWATER_FACTION_FLAG_COUNT] =
{
    { 192275, 1, -957.521f, -2165.91f, 127.953f, 1.72846f, 0, 0, 0.760596f, 0.649226f },
    { 192275, 1, -998.88f, -2228.72f, 92.4484f, 2.26929f, 0, 0, 0.906383f, 0.422456f },
    { 192275, 1, -1035.44f, -2249.63f, 102.171f, 5.29237f, 0, 0, 0.47539f, -0.879775f },
    { 192275, 1, -1096.02f, -2298.59f, 90.5079f, 4.70441f, 0, 0, 0.709921f, -0.704281f },
    { 192275, 1, -1164.81f, -2255.38f, 89.4484f, 3.76848f, 0, 0, 0.951277f, -0.308337f },
    { 192275, 1, -1189.35f, -2119.76f, 117.039f, 3.27892f, 0, 0, 0.997644f, -0.0686083f },
    { 192275, 1, -1132.53f, -2043, 86.9566f, 1.40705f, 0, 0, 0.646911f, 0.762565f },
    { 192275, 1, -1105.51f, -1981.77f, 88.7153f, 1.97101f, 0, 0, 0.833552f, 0.552441f },
    { 192275, 1, -1028.7f, -1969.9f, 112.327f, 1.38066f, 0, 0, 0.63679f, 0.771037f },
    { 192275, 1, -957.975f, -1998.67f, 88.7911f, 1.39462f, 0, 0, 0.642159f, 0.766572f },
    { 192275, 1, -903.871f, -1990.67f, 107.179f, 1.21464f, 0, 0, 0.570667f, 0.821182f },
    { 192275, 1, -960.855f, -2081.44f, 81.2855f, 1.73823f, 0, 0, 0.763759f, 0.645501f }
};

const RPCreature AllianceGuards[LUSHWATER_FACTION_GUARD_COUNT] =
{
    { ALLIANCE_GUARD_ENTRY, 1, -957.521f, -2165.91f, 127.953f, 1.72846f },
    { ALLIANCE_GUARD_ENTRY, 1, -998.88f, -2228.72f, 92.4484f, 2.26929f },
    { ALLIANCE_GUARD_ENTRY, 1, -1035.44f, -2249.63f, 102.171f, 5.29237f },
    { ALLIANCE_GUARD_ENTRY, 1, -1096.02f, -2298.59f, 90.5079f, 4.70441f },
    { ALLIANCE_GUARD_ENTRY, 1, -1164.81f, -2255.38f, 89.4484f, 3.76848f },
    { ALLIANCE_GUARD_ENTRY, 1, -1189.35f, -2119.76f, 117.039f, 3.27892f },
    { ALLIANCE_GUARD_ENTRY, 1, -1132.53f, -2043, 86.9566f, 1.40705f },
    { ALLIANCE_GUARD_ENTRY, 1, -1105.51f, -1981.77f, 88.7153f, 1.97101f },
    { ALLIANCE_GUARD_ENTRY, 1, -1028.7f, -1969.9f, 112.327f, 1.38066f },
    { ALLIANCE_GUARD_ENTRY, 1, -957.975f, -1998.67f, 88.7911f, 1.39462f },
    { ALLIANCE_GUARD_ENTRY, 1, -903.871f, -1990.67f, 107.179f, 1.21464f },
    { ALLIANCE_GUARD_ENTRY, 1, -960.855f, -2081.44f, 81.2855f, 1.73823f },
    { ALLIANCE_GUARD_ENTRY, 1, -957.521f, -2165.91f, 127.953f, 1.72846f },
    { ALLIANCE_GUARD_ENTRY, 1, -998.88f, -2228.72f, 92.4484f, 2.26929f },
    { ALLIANCE_GUARD_ENTRY, 1, -1035.44f, -2249.63f, 102.171f, 5.29237f },
    { ALLIANCE_GUARD_ENTRY, 1, -1096.02f, -2298.59f, 90.5079f, 4.70441f },
    { ALLIANCE_GUARD_ENTRY, 1, -1164.81f, -2255.38f, 89.4484f, 3.76848f },
    { ALLIANCE_GUARD_ENTRY, 1, -1189.35f, -2119.76f, 117.039f, 3.27892f },
    { ALLIANCE_GUARD_ENTRY, 1, -1132.53f, -2043, 86.9566f, 1.40705f },
    { ALLIANCE_GUARD_ENTRY, 1, -1105.51f, -1981.77f, 88.7153f, 1.97101f },
    { ALLIANCE_GUARD_ENTRY, 1, -1028.7f, -1969.9f, 112.327f, 1.38066f },
    { ALLIANCE_GUARD_ENTRY, 1, -957.975f, -1998.67f, 88.7911f, 1.39462f },
    { ALLIANCE_GUARD_ENTRY, 1, -903.871f, -1990.67f, 107.179f, 1.21464f },
    { ALLIANCE_GUARD_ENTRY, 1, -960.855f, -2081.44f, 81.2855f, 1.73823f }
};

const RPCreature HordeGuards[LUSHWATER_FACTION_GUARD_COUNT] =
{
    { HORDE_GUARD_ENTRY, 1, -957.521f, -2165.91f, 127.953f, 1.72846f },
    { HORDE_GUARD_ENTRY, 1, -998.88f, -2228.72f, 92.4484f, 2.26929f },
    { HORDE_GUARD_ENTRY, 1, -1035.44f, -2249.63f, 102.171f, 5.29237f },
    { HORDE_GUARD_ENTRY, 1, -1096.02f, -2298.59f, 90.5079f, 4.70441f },
    { HORDE_GUARD_ENTRY, 1, -1164.81f, -2255.38f, 89.4484f, 3.76848f },
    { HORDE_GUARD_ENTRY, 1, -1189.35f, -2119.76f, 117.039f, 3.27892f },
    { HORDE_GUARD_ENTRY, 1, -1132.53f, -2043, 86.9566f, 1.40705f },
    { HORDE_GUARD_ENTRY, 1, -1105.51f, -1981.77f, 88.7153f, 1.97101f },
    { HORDE_GUARD_ENTRY, 1, -1028.7f, -1969.9f, 112.327f, 1.38066f },
    { HORDE_GUARD_ENTRY, 1, -957.975f, -1998.67f, 88.7911f, 1.39462f },
    { HORDE_GUARD_ENTRY, 1, -903.871f, -1990.67f, 107.179f, 1.21464f },
    { HORDE_GUARD_ENTRY, 1, -960.855f, -2081.44f, 81.2855f, 1.73823f },
    { HORDE_GUARD_ENTRY, 1, -957.521f, -2165.91f, 127.953f, 1.72846f },
    { HORDE_GUARD_ENTRY, 1, -998.88f, -2228.72f, 92.4484f, 2.26929f },
    { HORDE_GUARD_ENTRY, 1, -1035.44f, -2249.63f, 102.171f, 5.29237f },
    { HORDE_GUARD_ENTRY, 1, -1096.02f, -2298.59f, 90.5079f, 4.70441f },
    { HORDE_GUARD_ENTRY, 1, -1164.81f, -2255.38f, 89.4484f, 3.76848f },
    { HORDE_GUARD_ENTRY, 1, -1189.35f, -2119.76f, 117.039f, 3.27892f },
    { HORDE_GUARD_ENTRY, 1, -1132.53f, -2043, 86.9566f, 1.40705f },
    { HORDE_GUARD_ENTRY, 1, -1105.51f, -1981.77f, 88.7153f, 1.97101f },
    { HORDE_GUARD_ENTRY, 1, -1028.7f, -1969.9f, 112.327f, 1.38066f },
    { HORDE_GUARD_ENTRY, 1, -957.975f, -1998.67f, 88.7911f, 1.39462f },
    { HORDE_GUARD_ENTRY, 1, -903.871f, -1990.67f, 107.179f, 1.21464f },
    { HORDE_GUARD_ENTRY, 1, -960.855f, -2081.44f, 81.2855f, 1.73823f }
};


class ResourcePoint_LushwaterOasis : public ResourcePoint
{
public:
    ResourcePoint_LushwaterOasis();
    ~ResourcePoint_LushwaterOasis() { }
    void Create() override;
    void Update(uint32 diff) override;
    void TakenControlBy(RPFaction faction);
    void SpawnAllianceUnits();
    void SpawnHordeUnits();
    uint32 CurrentProductionPower() override;
    uint32 CurrentDefensePower() override;
    uint32 CurrentMaxDefensePower() override;
    uint32 CurrentMaxProductionPower() override;
    uint32 CurrentReinforcePower() override;
    void SendDefensePowerUpdate() override;
    void SendProductionPowerUpdate() override;
    void UpdateWorker(uint32 diff);
    void UpdateGuard(uint32 diff);
    void Reset() override;
private:
    uint32 _testTimer;
    uint32 _workerUpdateTimer;
    uint32 _guardUpdateTimer;
};