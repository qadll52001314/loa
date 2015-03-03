#ifndef CAPITAL_CITY_MGR_H_
#define CAPITAL_CITY_MGR_H_

#include "CapitalCity.h"
#include "Player.h"

typedef std::map<uint32, CapitalCity*> CapitalCityMap;

enum CapitalCityArea
{
    CC_ORGRIMMAR = 1637,
    CC_THUNDERBLUFF = 1638,
    CC_UNDERCITY = 1497,
    CC_SILVERMOON = 3487,
    CC_STORMWIND = 1519,
    CC_IRONFORGE = 1537,
    CC_DARNASSUS = 1657,
    CC_EXODAR = 3557
};

class CapitalCityMgr
{
private:
    CapitalCityMgr() { }
    ~CapitalCityMgr() { }
public:
    static CapitalCityMgr* instance()
    {
        static CapitalCityMgr instance;
        return &instance;
    }

    void Update(uint32 diff);
    void LoadCapitalCities();
    CapitalCity* GetCapitalCity(uint32 area) const;
    void Save();

    void SendStateTo(Player*, uint32 index, uint32 value);
    void ClearStateOf(Player*, uint32 index);

    void HandlePlayerEnter(Player*, uint32 area);
    void HandlePlayerLeave(Player*, uint32 area);

    bool ReachedRequiredRank(Creature* creature, uint32 rank);
    CapitalCity* FactionBelongsTo(uint32 faction);
private:
    CapitalCityMap m_CapitalCities;
};

#define xCapitalCityMgr CapitalCityMgr::instance()

#endif

