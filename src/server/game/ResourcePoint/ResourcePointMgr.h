#ifndef RESOURCE_POINT_MGR_H_
#define RESOURCE_POINT_MGR_H_

#include "Util.h"
#include "SharedDefines.h"
#include "ResourcePoint.h"

#define RP_UPDATE_INVERVAL 1000

typedef std::map<uint32, ResourcePoint*> ResourcePointMap;
typedef std::map<uint32, uint32> ResourcePointScriptMap;

class ResourcePointMgr
{
private:
    ResourcePointMgr();
    ~ResourcePointMgr();
public:
    static ResourcePointMgr* instance()
    {
        static ResourcePointMgr instance;
        return &instance;
    }

    void Update(uint32 diff);

    void HandlePlayerEnter(Player* player, uint32 area);
    void HandlePlayerLeave(Player* player, uint32 area);

    void SendStateTo(Player* player, uint32 index, uint32 value);
    void ClearStateOf(Player* player, uint32 index);

    void HandleKilledCreature(uint32 area, Unit* killer, Creature* victim);

    ResourcePoint* GetResourcePoint(uint32 area);

    void LoadResourcePoints();

    void RegisterResourcePoint(uint32 area, ResourcePoint* point);

    void Save();

    // testing
    void SetupResourcePoints();
private:
    ResourcePointMap m_ResourcePoints;
    ResourcePointScriptMap m_ResourcePointScripts;
    uint32 m_UpdateTimer;
};

#define xResourcePointMgr ResourcePointMgr::instance()

#endif
