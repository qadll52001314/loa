#ifndef CAPITAL_CITY_MGR_H_
#define CAPITAL_CITY_MGR_H_

#include "CapitalCity.h"
#include "Player.h"

enum CapitalCityResearchWorldState
{
    WORLDSTATE_RESEARCH_PROGRESS = 6015,
    WORLDSTATE_RESEARCH_RANK = 6016,
    WORLDSTATE_RESEARCH_ITEM1_COUNT = 6017,
    WORLDSTATE_RESEARCH_ITEM2_COUNT = 6018,
    WORLDSTATE_RESEARCH_ITEM3_COUNT = 6019,
    WORLDSTATE_RESEARCH_ITEM4_COUNT = 6020,
    WORLDSTATE_RESEARCH_ITEM1_MAX = 6021,
    WORLDSTATE_RESEARCH_ITEM2_MAX = 6022,
    WORLDSTATE_RESEARCH_ITEM3_MAX = 6023,
    WORLDSTATE_RESEARCH_ITEM4_MAX = 6024,
    WORLDSTATE_RESEARCH_REQ_CITY_RANK = 6025
};

enum CapitalCityResearchStateCode
{
    CC_RESEARCH_STATE_NOT_STARTED = 0,
    CC_RESEARCH_STATE_IN_PROGRESS = 1,
    CC_RESEARCH_STATE_FINISHED = 2,
    CC_RESEARCH_STATE_STOPPED = 3,
    CC_RESEARCH_STATE_MAX
};

struct CapitalCityNpcResearchState
{
    uint32 spellSet;
    uint32 rank;
    uint8 state;
    uint32 progress;
    uint32 itemCount1;
    uint32 itemCount2;
    uint32 itemCount3;
    uint32 itemCount4;
};

struct CapitalCityUpgradeCost
{
    uint32 startResource;
    uint32 startMagicPower;
    uint32 resource;
    uint32 magicPower;
};

struct CapitalCityResearchSpell
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
    uint32 progress;
};

struct CapitalCityResearchState
{
    const CapitalCityResearchSpell* spell;
    uint8 state;
};

typedef std::map<uint32, CapitalCity*> CapitalCityMap;
typedef std::map<uint32, CapitalCityUpgradeCost> CapitalCityUpgradeCostMap;
typedef std::multimap<uint32, CapitalCityResearchSpell> CapitalCityResearchSpellMap;
typedef std::map<uint32, const CapitalCityResearchSpell*> PCapitalCityResearchSpellMap;
typedef std::multimap<uint32, CapitalCityNpcResearchState> CapitalCityResearchStateMap;
typedef std::multimap<uint32, uint32> CapitalCityResearcherSpellSetMap;
typedef std::multimap<uint32, uint32> CapitalCityLearnableSpellSetMap;
typedef std::map<uint32, std::string> CapitalCitySpellSetMap;
typedef std::map<uint32, std::string> CapitalCitySpellSetContainer;
typedef std::vector<const CapitalCityResearchSpell*> CapitalCityResearchSpellSetContainer;
typedef std::vector<uint32> CapitalCityResearchSpellSetList;

typedef std::pair<CapitalCityResearcherSpellSetMap::const_iterator, CapitalCityResearcherSpellSetMap::const_iterator> CapitalCityResearcherSpellSetConstBounds;
typedef std::pair<CapitalCityResearchSpellMap::const_iterator, CapitalCityResearchSpellMap::const_iterator> CapitalCityResearchSpellConstBounds;
typedef std::pair<CapitalCityResearchStateMap::const_iterator, CapitalCityResearchStateMap::const_iterator> CapitalCityResearchStateConstBounds;
typedef std::pair<CapitalCityResearchStateMap::iterator, CapitalCityResearchStateMap::iterator> CapitalCityResearchStateBounds;
typedef std::pair<CapitalCityLearnableSpellSetMap::const_iterator, CapitalCityLearnableSpellSetMap::const_iterator> CapitalCityLearnableSpellConstBounds;

typedef std::vector<CapitalCity*> CapitalCityList;
typedef std::vector<uint32> CapitalCitySpellList;

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
    void ResearchUpdate();
    void LoadCapitalCities();
    CapitalCity* GetCapitalCityByZone(uint32 zone) const;
    CapitalCity* GetCapitalCityByID(uint32 id) const;
    void Save();

    void SendStateTo(Player*, uint32 index, uint32 value) const;
    void ClearStateOf(Player*, uint32 index) const;

    void HandlePlayerEnter(Player*, uint32 zone);
    void HandlePlayerLeave(Player*, uint32 zone);

    bool ReachedRequiredRank(Creature* creature, uint32 rank) const;
    CapitalCity* FactionBelongsTo(uint32 faction) const;

    uint32 ResourceToNextLevel(uint32 nextLevel) const;
    uint32 MagicPowerToNextLevel(uint32 nextLevel) const;
    uint32 StartResourceToNextLevel(uint32 nextLevel) const;
    uint32 StartMagicPowerToNextLevel(uint32 nextLevel) const;

    CapitalCityResearchSpellSetContainer GetAvailableResearch(Creature* researcher) const;
    CapitalCityResearchSpellSetContainer GetInProgressResearch(Creature* researcher) const;
    CapitalCityResearchSpellSetContainer GetResearchCompleted(Creature* researcher) const;
    const CapitalCityResearchSpell* GetNextAvailableResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet) const;
    const CapitalCityResearchSpell* GetInProgressResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet) const;
    const CapitalCityResearchSpell* GetCompletedResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet) const;
    const CapitalCityResearchSpell* GetCapitalCityResearchSpell(uint32 spellSet, uint32 rank) const;

    CapitalCityResearchSpellSetList GetResearchListForCreatureEntry(uint32 entry) const;
    uint32 GetResearchSpellRank(uint32 spellSet, uint32 spell) const;
    const CapitalCityResearchSpell* GetResearchSpell(uint32 spellSet, uint32 rank) const;
    PCapitalCityResearchSpellMap GetSpellSet(uint32 spellset) const;
    CapitalCityResearchState GetResearchState(uint32 entry, uint32 spellSet) const;
    void UpdateResearchState(uint32 entry, uint32 spellSet, uint32 rank, uint8 state);
    bool CanCreatureResearch(uint32 creatureEntry, uint32 spellSet) const;
    CapitalCityList GetCapitalCitiesForTeam(uint32 team) const;
    CapitalCitySpellList GetLearnableSpellsForTeam(uint32 team) const;
    CapitalCitySpellList GetSpellsForRank(uint32 cityID, uint32 rank) const;
    void SendResearchProgress(Player* receiver, uint32 researcher, uint32 spellSet) const;
    const CapitalCityNpcResearchState* GetNpcResearchState(uint32 researcher, uint32 spellSet) const;
    uint32 GetResearchDescriptionText(uint32 spellSet, uint32 spell) const;
    std::string GetSpellSetName(uint32 spellSet) const;
    bool HaveAllReagentForNextResearch(uint32 researcherEntry, uint32 spellSet) const;
    void StartNextAvailableResearch(uint32 researcherEntry, uint32 spellSet);
    void AddReagentTo(uint32 researcherEntry, uint32 spellSet, uint32 item, uint32 count);
    CapitalCityNpcResearchState BuildNewResearchState(const CapitalCityResearchSpell* spell, uint32 spellSet, uint32 item, uint32 count) const;
private:
    CapitalCityMap m_CapitalCities;
    CapitalCityUpgradeCostMap m_UpgradeCosts;
    CapitalCityResearchSpellMap m_ResearchSpellMap;
    CapitalCityResearchStateMap m_ResearchStateMap;
    CapitalCityResearcherSpellSetMap m_ResearcherSpellSetMap;
    CapitalCityLearnableSpellSetMap m_LearnableSpellSetMap;
    CapitalCitySpellSetMap m_SpellSetMap;
};

#define xCapitalCityMgr CapitalCityMgr::instance()

#endif

