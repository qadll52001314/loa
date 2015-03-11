#ifndef CAPITAL_CITY_MGR_H_
#define CAPITAL_CITY_MGR_H_

#include "CapitalCity.h"
#include "Player.h"

struct CapitalCityUpgradeCost
{
    uint32 resource;
    uint32 magicPower;
};

struct CapitalCityUpgradeSpell
{
    uint32 rank;
    uint32 spell;
    uint32 description;
    uint32 reqCityRank;
    uint32 reqItem1;
    uint32 reqItem2;
    uint32 reqItem3;
    uint32 reqItem4;
    uint32 reqItemCount1;
    uint32 reqItemCount2;
    uint32 reqItemCount3;
    uint32 reqItemCount4;
};

typedef std::map<uint32, CapitalCity*> CapitalCityMap;
typedef std::map<uint32, CapitalCityUpgradeCost> CapitalCityUpgradeInfoMap;
typedef std::multimap<uint32, uint32> CapitalCityUpgradeSpellSet;
typedef std::map<uint32, CapitalCityUpgradeSpell> CapitalCityUpgradeSpellList;

enum CapitalCityIDs
{
    CC_ORGRIMMAR = 4,
    CC_THUNDERBLUFF = 5,
    CC_UNDERCITY = 1,
    CC_SILVERMOON = 7,
    CC_STORMWIND = 2,
    CC_IRONFORGE = 3,
    CC_DARNASSUS = 6,
    CC_EXODAR = 8
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

    void Update();
    void LoadCapitalCities();
    CapitalCity* GetCapitalCityByZone(uint32 zone) const;
    CapitalCity* GetCapitalCityByID(uint32 id) const;
    void Save();

    void SendStateTo(Player*, uint32 index, uint32 value);
    void ClearStateOf(Player*, uint32 index);

    void HandlePlayerEnter(Player*, uint32 zone);
    void HandlePlayerLeave(Player*, uint32 zone);

    bool ReachedRequiredRank(Creature* creature, uint32 rank);
    CapitalCity* FactionBelongsTo(uint32 faction);

    uint32 ResourceToNextLevel(uint32 nextLevel);
    uint32 MagicPowerToNextLevel(uint32 nextLevel);
private:
    CapitalCityMap m_CapitalCities;
    CapitalCityUpgradeInfoMap m_UpgradeInfo;
    CapitalCityUpgradeSpellSet m_UpgradeSpellSet;
    CapitalCityUpgradeSpellList m_UpgradeSpellList;
};

#define xCapitalCityMgr CapitalCityMgr::instance()

#endif

