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
    uint32 researchSet;
    uint32 rank;
    uint8 state;
    uint32 progress;
    uint32 itemCount1;
    uint32 itemCount2;
    uint32 itemCount3;
    uint32 itemCount4;
};

struct CapitalCityResearchData
{
    uint32 researchSet;
    uint32 rank;
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
    uint32 questSet;
    uint32 spell;
    uint32 item;
    uint32 progress;
};

struct CapitalCityResearchState
{
    const CapitalCityResearchData* data;
    uint8 state;
};

typedef std::map<uint32, CapitalCity*> CapitalCityMap;
typedef std::map<uint32, uint32> CapitalCityUpgradeCostMap;
typedef std::multimap<uint32, CapitalCityResearchData> CapitalCityResearchDataMap;
typedef std::map<uint32, const CapitalCityResearchData*> PCapitalCityResearchDataMap;
typedef std::multimap<uint32, CapitalCityNpcResearchState> CapitalCityResearchStateMap;
typedef std::multimap<uint32, uint32> CapitalCityLearnableResearchSetMap;
typedef std::map<uint32, std::string> CapitalCityResearchSetMap;
typedef std::map<uint32, std::string> CapitalCityResearchSetContainer;
typedef std::vector<const CapitalCityResearchData*> CapitalCityResearchDataSetContainer;
typedef std::map<uint32, const TrainerSpell*> CapitalCityTrainerSpellContainer;
typedef std::map<uint32, const VendorItem*> CapitalCityVendorItemContainer;

typedef std::multimap<uint32, uint32> CapitalCityResearchQuestMap;
typedef std::vector<uint32> CapitalCityResearchQuestList;
typedef std::map<uint32, VendorItem> CapitalCityVendorItemMap;

typedef std::pair<CapitalCityResearchDataMap::const_iterator, CapitalCityResearchDataMap::const_iterator> CapitalCityResearchDataConstBounds;
typedef std::pair<CapitalCityResearchStateMap::const_iterator, CapitalCityResearchStateMap::const_iterator> CapitalCityResearchStateConstBounds;
typedef std::pair<CapitalCityResearchStateMap::iterator, CapitalCityResearchStateMap::iterator> CapitalCityResearchStateBounds;
typedef std::pair<CapitalCityLearnableResearchSetMap::const_iterator, CapitalCityLearnableResearchSetMap::const_iterator> CapitalCityLearnableResearchConstBounds;

typedef std::vector<CapitalCity*> CapitalCityList;
typedef std::vector<uint32> CapitalCityResearchList;

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

    bool ReachedRequiredCityRank(Creature* creature, uint32 rank) const;
    CapitalCity* FactionBelongsTo(uint32 faction) const;

    uint32 ResourceToNextLevel(uint32 nextLevel) const;

    CapitalCityResearchDataSetContainer GetAvailableResearchSet(const Creature* researcher) const;
    CapitalCityResearchDataSetContainer GetInProgressResearchSet(const Creature* researcher) const;
    CapitalCityResearchDataSetContainer GetCompletedResearchSet(const Creature* researcher) const;

    const CapitalCityResearchData* GetAvailableResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const;
    const CapitalCityResearchData* GetAvailableResearchDataForCreature(Creature* creature, uint8 index) const;
    const CapitalCityResearchData* GetInProgressResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const;
    const CapitalCityResearchData* GetCompletedResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const;

    const CapitalCityResearchData* GetResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const;
    const CapitalCityResearchData* GetResearchDataForCreature(Creature* creature, uint8 index) const;

    const CapitalCityResearchData* GetCapitalCityResearchData(uint32 researchSet, uint32 rank) const;

    uint32 GetResearchDataRankFromSpell(uint32 researchSet, uint32 spell) const;
    const CapitalCityResearchData* GetResearchData(uint32 researchSet, uint32 rank) const;
    PCapitalCityResearchDataMap GetResearchSet(uint32 researchSet) const;
    CapitalCityResearchState GetResearchState(uint32 entry, uint32 researchSet) const;
    void UpdateResearchState(uint32 entry, uint32 researchSet, uint32 rank, uint8 state);
    void SaveResearchState(uint32 entry, const CapitalCityNpcResearchState* state) const;
    bool CanCreatureResearch(uint32 creatureEntry, uint32 researchSet) const;
    CapitalCityList GetCapitalCitiesForTeam(uint32 team) const;
    CapitalCityResearchList GetLearnableResearchSetForTeam(uint32 team) const;
    CapitalCityResearchList GetSpellsForRank(uint32 cityID, uint32 rank) const;
    void SendResearchProgress(Player* receiver, uint32 researcher, uint32 researchSet) const;
    const CapitalCityNpcResearchState* GetNpcResearchState(uint32 researcher, uint32 researchSet) const;
    const CapitalCityNpcResearchState* GetNpcResearchState(Creature* creature, uint8 index) const;
    uint32 GetResearchDescriptionText(uint32 researchSet, uint32 rank) const;
    std::string GetResearchSetName(uint32 researchSet) const;
    bool HaveAllReagentForNextResearch(uint32 researcherEntry, uint32 researchSet) const;
    bool HaveAllReagentForNextResearch(Creature* creature, uint8 index) const;
    void StartNextAvailableResearch(uint32 researcherEntry, uint32 researchSet);
    void StartNextAvailableResearch(Creature* creature, uint8 index);
    void AddReagentTo(uint32 researcherEntry, uint32 item, uint32 count);
    CapitalCityNpcResearchState BuildNewResearchState(const CapitalCityResearchData* data, uint32 researchSet, uint32 item, uint32 count) const;

    void PrepareResearchQuestMenu(Player* player, Creature* creature);
    void SendResearchTrainerList(Player* player, Creature* creature);

    CapitalCityResearchQuestList GetQuestSet(uint32 questSet) const;

    bool HaveResearchQuest(const Creature* creature, uint32 questID) const;
    CapitalCityTrainerSpellContainer GetTrainableSpells(Creature* creature);
    const TrainerSpell* GetTrainerSpell(uint32 spell);
    CapitalCityVendorItemContainer GetVendorItems(Creature* creature);
    const VendorItem* GetVendorItem(uint32 item);
    bool HaveAvailableResearch(uint32 entry) const;
    void StartResearch(Creature* creature, uint8 index, uint32 announceTeam);
private:
    CapitalCityMap m_CapitalCities;
    CapitalCityUpgradeCostMap m_UpgradeCosts;
    CapitalCityResearchDataMap m_ResearchDataMap;
    CapitalCityResearchStateMap m_ResearchStateMap;
    CapitalCityLearnableResearchSetMap m_LearnableResearchSetMap;
    CapitalCityResearchSetMap m_ResearchSetMap;
    CapitalCityResearchQuestMap m_ResearchQuestMap;
    TrainerSpellMap m_TrainerSpellMap;
    CapitalCityVendorItemMap m_VendorItemMap;
};

#define xCapitalCityMgr CapitalCityMgr::instance()

#endif

