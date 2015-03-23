#include "CapitalCityMgr.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "GossipDef.h"

void CapitalCityMgr::LoadCapitalCities()
{
    m_CapitalCities.clear();

    uint32 oldMSTime = getMSTime();


    QueryResult result = WorldDatabase.Query("SELECT ID, Zone, Faction, StartLevel, ScriptName FROM capital_cities");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Capital City definitions. DB table `capital_cities` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 scriptId = sObjectMgr->GetScriptId(fields[4].GetCString());
        CapitalCity* city = sScriptMgr->CreateCapitalCity(scriptId);
        if (city)
        {
            city->SetID(fields[0].GetUInt32());
            city->SetZone(fields[1].GetUInt32());
            city->SetTeam(fields[2].GetUInt32());
			city->SetRank(fields[3].GetUInt32());
            m_CapitalCities[fields[0].GetUInt32()] = city;
            ++count;
        }
    } while (result->NextRow());

    result = CharacterDatabase.Query("SELECT ID, Resource, MagicPower, Level, UpgradeResource, UpgradeMagicPower, Upgrading FROM capital_city_state");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            if (CapitalCity* city = GetCapitalCityByID(fields[0].GetUInt32()))
            {
                city->SetResource(fields[1].GetUInt32());
				city->SetMagicPower(fields[2].GetUInt32());
                city->SetRank(fields[3].GetUInt32() > city->GetRank() ? fields[3].GetUInt32() : city->GetRank());
                city->SetUpgradeResource(fields[4].GetUInt32());
                city->SetUpgradeMagicPower(fields[5].GetUInt32());
                city->SetUpgradeState(fields[6].GetBool());
            }
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    m_UpgradeCosts.clear();

    result = WorldDatabase.Query("SELECT Level, StartResource, StartMagicPower, Resource, MagicPower FROM capital_city_upgrade_cost");

    count = 0;

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CapitalCityUpgradeCost info;
            info.startResource = fields[1].GetUInt32();
            info.startMagicPower = fields[2].GetUInt32();
            info.resource = fields[3].GetUInt32();
            info.magicPower = fields[4].GetUInt32();
            m_UpgradeCosts[fields[0].GetUInt32()] = info;
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City upgrade cost informations", count);

    m_ResearchSetMap.clear();

    count = 0;

    result = WorldDatabase.Query("SELECT ID, Name FROM capital_city_research_set");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_ResearchSetMap[fields[0].GetUInt32()] = fields[1].GetString();
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Research Set", count);

    m_ResearchDataMap.clear();

    result = WorldDatabase.Query("SELECT ResearchSet, Rank, Description, ReqCityRank, ReqItem1, ReqItem2, ReqItem3, ReqItem4, ReqItemCount1, ReqItemCount2, ReqItemCount3, ReqItemCount4, QuestSet, Spell, Item, Progress FROM capital_city_research_data");

    count = 0;

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            CapitalCityResearchData data;
            data.researchSet = fields[0].GetUInt32();
            data.rank = fields[1].GetUInt32();
            data.description = fields[2].GetUInt32();
            data.reqCityRank = fields[3].GetUInt32();
            data.reqItem1 = fields[4].GetUInt32();
            data.reqItem2 = fields[5].GetUInt32();
            data.reqItem3 = fields[6].GetUInt32();
            data.reqItem4 = fields[7].GetUInt32();
            data.reqItemCount1 = fields[8].GetUInt32();
            data.reqItemCount2 = fields[9].GetUInt32();
            data.reqItemCount3 = fields[10].GetUInt32();
            data.reqItemCount4 = fields[11].GetUInt32();
            data.questSet = fields[12].GetUInt32();
            data.spell = fields[13].GetUInt32();
            data.item = fields[14].GetUInt32();
            data.progress = fields[15].GetUInt32();
            if (!data.progress)
                data.progress = 86400; // 1 day
            m_ResearchDataMap.insert(std::pair<uint32, CapitalCityResearchData>(fields[0].GetUInt32(), data));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Research Data", count);

    m_TrainerSpellMap.clear();

    result = WorldDatabase.Query("SELECT Spell, Cost, ReqSkill, ReqSkillValue, ReqLevel, ReqCityRank FROM capital_city_research_trainer_data");

    count = 0;

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            TrainerSpell spell;
            spell.spell = fields[0].GetUInt32();
            spell.spellCost = fields[1].GetUInt32();
            spell.reqSkill = fields[2].GetUInt32();
            spell.reqSkillValue = fields[3].GetUInt32();
            spell.reqLevel = fields[4].GetUInt32();
            spell.reqCityRank = fields[5].GetUInt32();
            spell.learnedSpell[0] = spell.spell;
            spell.learnedSpell[1] = 0;
            spell.learnedSpell[2] = 0;
            m_TrainerSpellMap[fields[0].GetUInt32()] = spell;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Trainer Spell Data", count);

    count = 0;

    result = CharacterDatabase.Query("SELECT Entry, ResearchSet, Rank, State, Progress, ItemCount1, ItemCount2, ItemCount3, ItemCount4 FROM capital_city_research_state");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CapitalCityNpcResearchState state;
            state.researchSet = fields[1].GetUInt32();
            state.rank = fields[2].GetUInt32();
            state.state = fields[3].GetUInt8();
            state.progress = fields[4].GetUInt8();
            state.itemCount1 = fields[5].GetUInt32();
            state.itemCount2 = fields[6].GetUInt32();
            state.itemCount3 = fields[7].GetUInt32();
            state.itemCount4 = fields[8].GetUInt32();
            if (state.state >= CC_RESEARCH_STATE_MAX)
                state.state = CC_RESEARCH_STATE_NOT_STARTED;
            m_ResearchStateMap.insert(std::pair<uint32, CapitalCityNpcResearchState>(fields[0].GetUInt32(), state));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Research States", count);

    count = 0;

    result = WorldDatabase.Query("SELECT City, ResearchSet FROM capital_city_rankup_research_set");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_LearnableResearchSetMap.insert(std::pair<uint32, uint32>(fields[0].GetUInt32(), fields[1].GetUInt32()));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Ranking Up Research Set", count);

    m_ResearchQuestMap.clear();

    count = 0;

    result = WorldDatabase.Query("SELECT QuestSet, Quest FROM capital_city_research_quest");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_ResearchQuestMap.insert(std::pair<uint32, uint32>(fields[0].GetUInt32(), fields[1].GetUInt32()));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Research Quest", count);

    m_VendorItemMap.clear();

    count = 0;

    result = WorldDatabase.Query("SELECT Item, MaxCount, IncrTime, ItemExtendedCost, ReqCityRank FROM capital_city_research_vendor_data");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            VendorItem item;
            item.item = fields[0].GetUInt32();
            item.maxcount = fields[1].GetUInt32();
            item.incrtime = fields[2].GetUInt32();
            item.ExtendedCost = fields[3].GetUInt32();
            item.ReqCityRank = fields[4].GetUInt32();
            m_VendorItemMap[fields[0].GetUInt32()] = item;
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City Research Vendor Data", count);
}

void CapitalCityMgr::Save()
{
    for (CapitalCityMap::iterator itr = m_CapitalCities.begin(); itr != m_CapitalCities.end(); ++itr)
    {
        if (itr->second)
        {
            itr->second->Save();
            delete itr->second;
        }
    }
}

void CapitalCityMgr::SendStateTo(Player* player, uint32 index, uint32 value) const
{
    player->SendUpdateWorldState(index, value);
}

void CapitalCityMgr::ClearStateOf(Player* player, uint32 index) const
{
    player->SendUpdateWorldState(index, 0);
}

void CapitalCityMgr::HandlePlayerEnter(Player* player, uint32 zone)
{
    if (CapitalCity* city = GetCapitalCityByZone(zone))
        city->HandlePlayerEnter(player);
}

void CapitalCityMgr::HandlePlayerLeave(Player* player, uint32 zone)
{
    if (CapitalCity* city = GetCapitalCityByZone(zone))
        city->HandlePlayerLeave(player);
}

CapitalCity* CapitalCityMgr::GetCapitalCityByZone(uint32 zone) const
{
    for (CapitalCityMap::const_iterator itr = m_CapitalCities.begin(); itr != m_CapitalCities.end(); ++itr)
    {
        if (itr->second->GetZone() == zone)
            return itr->second;
    }
    return NULL;
}

CapitalCity* CapitalCityMgr::GetCapitalCityByID(uint32 id) const
{
    CapitalCityMap::const_iterator itr = m_CapitalCities.find(id);
    if (itr == m_CapitalCities.end())
        return NULL;
    return itr->second;
}

bool CapitalCityMgr::ReachedRequiredCityRank(Creature* creature, uint32 rank) const
{
    if (rank == 0)
        return true;

    if (!creature)
        return false;

    CapitalCity* city = FactionBelongsTo(creature->GetFactionTemplateEntry()->faction);

    if (!city)
        return false;

    return city->GetRank() >= rank;
}

CapitalCity* CapitalCityMgr::FactionBelongsTo(uint32 faction) const
{
    switch (faction)
    {
    case 76:
    case 530: // darkspear troll
        return GetCapitalCityByID(CC_ORGRIMMAR);
    case 81:
        return GetCapitalCityByID(CC_THUNDERBLUFF);
    case 68:
        return GetCapitalCityByID(CC_UNDERCITY);
    case 911:
        return GetCapitalCityByID(CC_SILVERMOON);
    case 72:
        return GetCapitalCityByID(CC_STORMWIND);
    case 47:
    case 54:
        return GetCapitalCityByID(CC_IRONFORGE);
    case 69:
        return GetCapitalCityByID(CC_DARNASSUS);
    case 930:
        return GetCapitalCityByID(CC_EXODAR);
    default:
        return NULL;
    }
}

void CapitalCityMgr::Update()
{
    for (CapitalCityMap::iterator itr = m_CapitalCities.begin(); itr != m_CapitalCities.end(); ++itr)
    {
        if (itr->second)
            itr->second->Update();
    }
}

uint32 CapitalCityMgr::ResourceToNextLevel(uint32 nextLevel) const
{
    CapitalCityUpgradeCostMap::const_iterator itr = m_UpgradeCosts.find(nextLevel);
    if (itr == m_UpgradeCosts.end())
        return 0;
    return itr->second.resource;
}

uint32 CapitalCityMgr::MagicPowerToNextLevel(uint32 nextLevel) const
{
    CapitalCityUpgradeCostMap::const_iterator itr = m_UpgradeCosts.find(nextLevel);
    if (itr == m_UpgradeCosts.end())
        return 0;
    return itr->second.magicPower;
}

uint32 CapitalCityMgr::StartResourceToNextLevel(uint32 nextLevel) const
{
    CapitalCityUpgradeCostMap::const_iterator itr = m_UpgradeCosts.find(nextLevel);
    if (itr == m_UpgradeCosts.end())
        return 0;
    return itr->second.startResource;
}

uint32 CapitalCityMgr::StartMagicPowerToNextLevel(uint32 nextLevel) const
{
    CapitalCityUpgradeCostMap::const_iterator itr = m_UpgradeCosts.find(nextLevel);
    if (itr == m_UpgradeCosts.end())
        return 0;
    return itr->second.startMagicPower;
}

CapitalCityResearchDataSetContainer CapitalCityMgr::GetAvailableResearchSet(const Creature* researcher) const
{
    CapitalCityResearchDataSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    const CreatureTemplate* proto = researcher->GetCreatureTemplate();

    uint32 entry = researcher->GetEntry();

    for (uint8 i = 0; i != MAX_CREATURE_RESEARCHSET; ++i)
    {
        const CapitalCityResearchData* data = GetAvailableResearchDataForCreature(entry, proto->ResearchSet[i]);
        if (data)
            container.push_back(data);
    }

    return container;
}

CapitalCityResearchDataSetContainer CapitalCityMgr::GetInProgressResearchSet(const Creature* researcher) const
{
    CapitalCityResearchDataSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    const CreatureTemplate* proto = researcher->GetCreatureTemplate();

    uint32 entry = researcher->GetEntry();

    for (uint8 i = 0; i != MAX_CREATURE_RESEARCHSET; ++i)
    {
        const CapitalCityResearchData* data = GetInProgressResearchDataForCreature(entry, proto->ResearchSet[i]);
        if (data)
            container.push_back(data);
    }

    return container;
}

uint32 CapitalCityMgr::GetResearchDataRankFromSpell(uint32 researchSet, uint32 spell) const
{
    CapitalCityResearchDataConstBounds bound = m_ResearchDataMap.equal_range(researchSet);
    if (bound.first == bound.second)
        return 0;

    for (CapitalCityResearchDataMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spell == spell)
            return itr->second.rank;
    }

    return 0;
}

PCapitalCityResearchDataMap CapitalCityMgr::GetResearchSet(uint32 researchSet) const
{
    PCapitalCityResearchDataMap set;
    CapitalCityResearchDataConstBounds bound = m_ResearchDataMap.equal_range(researchSet);
    if (bound.first == bound.second)
        return set;

    for (CapitalCityResearchDataMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        set.insert(std::pair<uint32, const CapitalCityResearchData*>(itr->second.rank, &itr->second));

    return set;
}

const CapitalCityResearchData* CapitalCityMgr::GetAvailableResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const
{
    if (!CanCreatureResearch(creatureEntry, researchSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, researchSet);
    if (!state.data)
        return GetResearchData(researchSet, 1);

    if (state.state == CC_RESEARCH_STATE_NOT_STARTED)
        return state.data;

    return NULL;
}

const CapitalCityResearchData* CapitalCityMgr::GetResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const
{
    if (!CanCreatureResearch(creatureEntry, researchSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, researchSet);
    if (!state.data)
        return GetResearchData(researchSet, 1);

    return state.data;
}

const CapitalCityResearchData* CapitalCityMgr::GetResearchDataForCreature(Creature* creature, uint8 index) const
{
    if (!creature)
        return NULL;

    if (index >= MAX_CREATURE_RESEARCHSET)
        return NULL;

    const CreatureTemplate* proto = creature->GetCreatureTemplate();
    if (!proto)
        return NULL;

    uint32 researchSet = proto->ResearchSet[index];

    CapitalCityResearchStateConstBounds bound = m_ResearchStateMap.equal_range(creature->GetEntry());
    for (CapitalCityResearchStateMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.researchSet == researchSet)
            return GetResearchDataForCreature(creature->GetEntry(), researchSet);
    }

    return GetResearchData(researchSet, 1);
}

const CapitalCityResearchData* CapitalCityMgr::GetInProgressResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const
{
    if (!CanCreatureResearch(creatureEntry, researchSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, researchSet);
    if (!state.data)
        return NULL;

    if (state.state == CC_RESEARCH_STATE_IN_PROGRESS)
        return state.data;

    return NULL;
}

const CapitalCityResearchData* CapitalCityMgr::GetCompletedResearchDataForCreature(uint32 creatureEntry, uint32 researchSet) const
{
    if (!CanCreatureResearch(creatureEntry, researchSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, researchSet);
    if (!state.data)
        return NULL;

    if (state.state == CC_RESEARCH_STATE_FINISHED)
        return state.data;

    return NULL;
}

CapitalCityResearchState CapitalCityMgr::GetResearchState(uint32 entry, uint32 researchSet) const
{
    CapitalCityResearchState state;
    state.data = NULL;
    state.state = CC_RESEARCH_STATE_NOT_STARTED;

    CapitalCityResearchStateConstBounds stateBound = m_ResearchStateMap.equal_range(entry);
    if (stateBound.first == stateBound.second)
    {
        // not started
        state.data = GetResearchData(researchSet, 1);
    }
    else
    {
        for (CapitalCityResearchStateMap::const_iterator itr = stateBound.first; itr != stateBound.second; ++itr)
        {
            if (itr->second.researchSet == researchSet)
            {
                state.state = itr->second.state;
                state.data = GetResearchData(researchSet, itr->second.rank);
            }
        }
    }

    return state;
}

bool CapitalCityMgr::CanCreatureResearch(uint32 creatureEntry, uint32 researchSet) const
{
    const CreatureTemplate* proto = sObjectMgr->GetCreatureTemplate(creatureEntry);

    if (!proto)
        return false;

    for (uint8 i = 0; i != MAX_CREATURE_RESEARCHSET; ++i)
    {
        if (proto->ResearchSet[i] == researchSet)
            return true;
    }
    return false;
}

CapitalCityResearchDataSetContainer CapitalCityMgr::GetCompletedResearchSet(const Creature* researcher) const
{
    CapitalCityResearchDataSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    const CreatureTemplate* proto = researcher->GetCreatureTemplate();

    for (uint8 i = 0; i != MAX_CREATURE_RESEARCHSET; ++i)
    {
        const CapitalCityResearchData* data = GetCompletedResearchDataForCreature(researcher->GetEntry(), proto->ResearchSet[i]);
        if (data)
            container.push_back(data);
    }

    return container;
}

void CapitalCityMgr::UpdateResearchState(uint32 entry, uint32 researchSet, uint32 rank, uint8 state)
{
    CapitalCityResearchStateBounds bound = m_ResearchStateMap.equal_range(entry);
    if (bound.first == bound.second)
        return;

    for (CapitalCityResearchStateMap::iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.researchSet == researchSet)
        {
            itr->second.rank = rank;
            itr->second.state = state;
            SaveResearchState(entry, &itr->second);
            return;
        }
    }

    // no result found, create one.
    CapitalCityNpcResearchState newState;
    newState.researchSet = researchSet;
    newState.rank = rank;
    newState.state = state;
    newState.progress = 0;
    newState.itemCount1 = 0;
    newState.itemCount2 = 0;
    newState.itemCount3 = 0;
    newState.itemCount4 = 0;
    m_ResearchStateMap.insert(std::pair<uint32, CapitalCityNpcResearchState>(entry, newState));

    SaveResearchState(entry, &newState);
}

const CapitalCityResearchData* CapitalCityMgr::GetResearchData(uint32 researchSet, uint32 rank) const
{
    CapitalCityResearchDataConstBounds bound = m_ResearchDataMap.equal_range(researchSet);
    if (bound.first == bound.second)
        return NULL;

    for (CapitalCityResearchDataMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.rank == rank)
            return &itr->second;
    }

    return NULL;
}

CapitalCityList CapitalCityMgr::GetCapitalCitiesForTeam(uint32 team) const
{
    CapitalCityList v;
    for (CapitalCityMap::const_iterator itr = m_CapitalCities.begin(); itr != m_CapitalCities.end(); ++itr)
    {
        if (itr->second->GetTeam() == 0 || itr->second->GetTeam() == team)
            v.push_back(itr->second);
    }
    return v;
}

CapitalCityResearchList CapitalCityMgr::GetLearnableResearchSetForTeam(uint32 team) const
{
    CapitalCityResearchList v;

    CapitalCityList list = GetCapitalCitiesForTeam(team);

    for (int i = 0; i != list.size(); ++i)
    {
        CapitalCity* city = list[i];

        CapitalCityLearnableResearchConstBounds bound = m_LearnableResearchSetMap.equal_range(city->GetID());
        if (bound.first == bound.second)
            continue;

        for (CapitalCityLearnableResearchSetMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            PCapitalCityResearchDataMap data = GetResearchSet(itr->second);
            if (!data.empty())
            {
                uint32 maxRank = 0;
                uint32 maxSpell = 0;

                for (PCapitalCityResearchDataMap::const_iterator itrSpell = data.begin(); itrSpell != data.end(); ++itrSpell)
                {
                    if (itrSpell->second->rank > maxRank && itrSpell->second->reqCityRank <= city->GetRank())
                    {
                        maxRank = itrSpell->second->rank;
                        maxSpell = itrSpell->second->spell;
                    }
                }

                if (maxRank && maxSpell)
                    v.push_back(maxSpell); // @todo: check dulplicate conditions
            }
        }
    }

    return v;
}

CapitalCityResearchList CapitalCityMgr::GetSpellsForRank(uint32 cityID, uint32 rank) const
{
    CapitalCityResearchList list;
    CapitalCity* city = GetCapitalCityByID(cityID);
    if (!city)
        return list;

    CapitalCityLearnableResearchConstBounds bound = m_LearnableResearchSetMap.equal_range(cityID);
    if (bound.first == bound.second)
        return list;

    for (CapitalCityLearnableResearchSetMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        PCapitalCityResearchDataMap set = GetResearchSet(itr->second);
        if (set.empty())
            continue;

        for (PCapitalCityResearchDataMap::const_iterator itrSet = set.begin(); itrSet != set.end(); ++itrSet)
        {
            if (itrSet->second->reqCityRank == rank)
                list.push_back(itrSet->second->spell);
        }
    }

    return list;
}

void CapitalCityMgr::SendResearchProgress(Player* receiver, uint32 researcher, uint32 researchSet) const
{
    const CapitalCityNpcResearchState* state = GetNpcResearchState(researcher, researchSet);

    const CapitalCityResearchData* data = GetCapitalCityResearchData(researchSet, state ? state->rank : 1);
    if (!data)
        return;

    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_PROGRESS, state ? state->progress * 100 / data->progress : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_RANK, state ? state->rank : 1);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM1_COUNT, state ? state->itemCount1 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM2_COUNT, state ? state->itemCount2 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM3_COUNT, state ? state->itemCount3 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM4_COUNT, state ? state->itemCount4 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM1_MAX, data->reqItemCount1);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM2_MAX, data->reqItemCount2);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM3_MAX, data->reqItemCount3);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM4_MAX, data->reqItemCount4);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_REQ_CITY_RANK, data->reqCityRank);
}

uint32 CapitalCityMgr::GetResearchDescriptionText(uint32 researchSet, uint32 rank) const
{
    PCapitalCityResearchDataMap set = GetResearchSet(researchSet);
    if (set.empty())
        return 0;

    for (PCapitalCityResearchDataMap::const_iterator itr = set.begin(); itr != set.end(); ++itr)
    {
        if (itr->second->rank == rank)
            return itr->second->description;
    }

    return 0;
}

const CapitalCityNpcResearchState* CapitalCityMgr::GetNpcResearchState(uint32 researcher, uint32 researchSet) const
{
    CapitalCityResearchStateConstBounds bound = m_ResearchStateMap.equal_range(researcher);
    if (bound.first != bound.second)
    {
        for (CapitalCityResearchStateMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.researchSet == researchSet)
                return &itr->second;
        }
    }
    return NULL;
}

const CapitalCityResearchData* CapitalCityMgr::GetCapitalCityResearchData(uint32 researchSet, uint32 rank) const
{
    CapitalCityResearchDataConstBounds bound = m_ResearchDataMap.equal_range(researchSet);
    if (bound.first != bound.second)
    {
        for (CapitalCityResearchDataMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.rank == rank)
                return &itr->second;
        }
    }
    return NULL;
}

std::string CapitalCityMgr::GetResearchSetName(uint32 researchSet) const
{
    CapitalCityResearchSetMap::const_iterator itr = m_ResearchSetMap.find(researchSet);
    if (itr != m_ResearchSetMap.end())
        return itr->second;
    return "";
}

bool CapitalCityMgr::HaveAllReagentForNextResearch(uint32 researcherEntry, uint32 researchSet) const
{
    const CapitalCityResearchData* data = GetAvailableResearchDataForCreature(researcherEntry, researchSet);
    if (!data)
        return false;

    const CapitalCityNpcResearchState* state = GetNpcResearchState(researcherEntry, researchSet);
    if (!state)
        return false;

    return state->itemCount1 >= data->reqItemCount1
        && state->itemCount2 >= data->reqItemCount2
        && state->itemCount3 >= data->reqItemCount3
        && state->itemCount4 >= data->reqItemCount4;
}

bool CapitalCityMgr::HaveAllReagentForNextResearch(Creature* creature, uint8 index) const
{
    if (!creature)
        return false;

    if (index >= MAX_CREATURE_RESEARCHSET)
        return false;

    const CreatureTemplate* proto = creature->GetCreatureTemplate();
    if (!proto)
        return false;

    return HaveAllReagentForNextResearch(creature->GetEntry(), proto->ResearchSet[index]);
}

void CapitalCityMgr::StartNextAvailableResearch(uint32 researcherEntry, uint32 researchSet)
{
    if (!HaveAllReagentForNextResearch(researcherEntry, researchSet))
        return;

    CapitalCityResearchStateBounds bound = m_ResearchStateMap.equal_range(researcherEntry);
    if (bound.first == bound.second) // shouldnt happen
        return;

    for (CapitalCityResearchStateMap::iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.researchSet == researchSet && itr->second.state == CC_RESEARCH_STATE_NOT_STARTED)
        {
            UpdateResearchState(researcherEntry, researchSet, itr->second.rank, CC_RESEARCH_STATE_IN_PROGRESS);
            return;
        }
    }
}

void CapitalCityMgr::ResearchUpdate()
{
    // this method will be called every 1 minute, so just +1 to all research in progress.
    for (CapitalCityResearchStateMap::iterator itr = m_ResearchStateMap.begin(); itr != m_ResearchStateMap.end(); ++itr)
    {
        if (itr->second.state == CC_RESEARCH_STATE_IN_PROGRESS)
        {
            itr->second.progress += 1;

            const CapitalCityResearchData* data = GetCapitalCityResearchData(itr->second.researchSet, itr->second.rank);

            ASSERT(data);

            if (itr->second.progress >= data->progress)
            {
                const CapitalCityResearchData* nextSpell = GetCapitalCityResearchData(itr->second.researchSet, itr->second.rank + 1);
                {
                    if (!nextSpell) // last research of this spellset
                        itr->second.state = CC_RESEARCH_STATE_FINISHED;
                    else
                    {
                        itr->second.rank += 1;
                        itr->second.progress = 0;
                        itr->second.itemCount1 = 0;
                        itr->second.itemCount2 = 0;
                        itr->second.itemCount3 = 0;
                        itr->second.itemCount4 = 0;
                        itr->second.state = CC_RESEARCH_STATE_NOT_STARTED;
                    }
                }
            }

            SaveResearchState(itr->first, &itr->second);
        }
    }
}

void CapitalCityMgr::AddReagentTo(uint32 researcherEntry, uint32 item, uint32 count)
{
    if (!item || !count)
        return;

    const CreatureTemplate* proto = sObjectMgr->GetCreatureTemplate(researcherEntry);
    if (!proto)
        return;

    CapitalCityResearchStateBounds bound = m_ResearchStateMap.equal_range(researcherEntry);

    for (uint8 i = 0; i != MAX_CREATURE_RESEARCHSET; ++i)
    {
        const CapitalCityResearchData* data = GetAvailableResearchDataForCreature(researcherEntry, proto->ResearchSet[i]);
        if (!data)
            continue;

        if (data->reqItem1 != item && data->reqItem2 != item && data->reqItem3 != item && data->reqItem4 != item)
            continue;

        if (bound.first == bound.second)
        {
            CapitalCityNpcResearchState state = BuildNewResearchState(data, proto->ResearchSet[i], item, count);
            m_ResearchStateMap.insert(std::pair<uint32, CapitalCityNpcResearchState>(researcherEntry, state));
        }
        else
        {
            for (CapitalCityResearchStateMap::iterator itrResearch = bound.first; itrResearch != bound.second; ++itrResearch)
            {
                if (itrResearch->second.researchSet == proto->ResearchSet[i])
                {
                    if (data->reqItem1 == item)
                        itrResearch->second.itemCount1 += count;
                    else if (data->reqItem2 == item)
                        itrResearch->second.itemCount2 += count;
                    else if (data->reqItem3 == item)
                        itrResearch->second.itemCount3 += count;
                    else if (data->reqItem4 == item)
                        itrResearch->second.itemCount4 += count;
                    SaveResearchState(researcherEntry, &itrResearch->second);
                    return;
                }
            }

            CapitalCityNpcResearchState state = BuildNewResearchState(data, proto->ResearchSet[i], item, count);
            SaveResearchState(researcherEntry, &state);
            m_ResearchStateMap.insert(std::pair<uint32, CapitalCityNpcResearchState>(researcherEntry, state));
        }

        return;
    }
}

void CapitalCityMgr::SaveResearchState(uint32 entry, const CapitalCityNpcResearchState* state) const
{
    if (!entry || !state)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CAPITAL_CITY_RESEARCH_STATE);
    stmt->setUInt32(0, entry);
    stmt->setUInt32(1, state->researchSet);
    stmt->setUInt32(2, state->rank);
    stmt->setUInt8(3, state->state);
    stmt->setUInt32(4, state->progress);
    stmt->setUInt32(5, state->itemCount1);
    stmt->setUInt32(6, state->itemCount2);
    stmt->setUInt32(7, state->itemCount3);
    stmt->setUInt32(8, state->itemCount4);
    CharacterDatabase.Execute(stmt);
}

CapitalCityNpcResearchState CapitalCityMgr::BuildNewResearchState(const CapitalCityResearchData* data, uint32 researchSet, uint32 item, uint32 count) const
{
    ASSERT(data);

    CapitalCityNpcResearchState state;
    state.itemCount1 = 0;
    state.itemCount2 = 0;
    state.itemCount3 = 0;
    state.itemCount4 = 0;
    state.progress = 0;
    state.rank = 1;
    state.researchSet = researchSet;
    state.state = CC_RESEARCH_STATE_NOT_STARTED;

    if (data->reqItem1 == item)
        state.itemCount1 += count;
    else if (data->reqItem2 == item)
        state.itemCount2 += count;
    else if (data->reqItem3 == item)
        state.itemCount3 += count;
    else if (data->reqItem4 == item)
        state.itemCount4 += count;

    return state;
}

void CapitalCityMgr::PrepareResearchQuestMenu(Player* player, Creature* creature)
{
    CapitalCityResearchDataSetContainer container = GetAvailableResearchSet(creature);

    if (container.empty())
        return;

    QuestMenu &qm = player->PlayerTalkClass->GetQuestMenu();
    qm.ClearMenu();

    for (CapitalCityResearchDataSetContainer::const_iterator itr = container.begin(); itr != container.end(); ++itr)
    {
        uint32 questSet = (*itr)->questSet;
        CapitalCityResearchQuestList list = GetQuestSet(questSet);
        if (list.empty())
            continue;
        for (int i = 0; i != list.size(); ++i)
        {
            uint32 questID = list[i];
            Quest const* quest = sObjectMgr->GetQuestTemplate(questID);
            if (!quest)
                continue;
            QuestStatus status = player->GetQuestStatus(questID);
            if (status == QUEST_STATUS_COMPLETE)
                qm.AddMenuItem(questID, 4);
            else if (status == QUEST_STATUS_INCOMPLETE)
                qm.AddMenuItem(questID, 4);
            if (!player->CanTakeQuest(quest, true))
                continue;
            qm.AddMenuItem(questID, 2);
        }
    }
}

CapitalCityResearchQuestList CapitalCityMgr::GetQuestSet(uint32 questSet) const
{
    CapitalCityResearchQuestList list;
    std::pair<CapitalCityResearchQuestMap::const_iterator, CapitalCityResearchQuestMap::const_iterator> bound = m_ResearchQuestMap.equal_range(questSet);
    if (bound.first == bound.second)
        return list;

    for (CapitalCityResearchQuestMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        list.push_back((*itr).second);

    return list;
}

bool CapitalCityMgr::HaveResearchQuest(const Creature* creature, uint32 questID) const
{
    if (!creature)
        return false;

    CapitalCityResearchDataSetContainer container = GetAvailableResearchSet(creature);
    if (container.empty())
        return false;

    for (CapitalCityResearchDataSetContainer::const_iterator itr = container.begin(); itr != container.end(); ++itr)
    {
        uint32 questSet = (*itr)->questSet;
        CapitalCityResearchQuestList list = GetQuestSet(questSet);
        if (list.empty())
            continue;
        for (int i = 0; i != list.size(); ++i)
        {
            if (questID == list[i])
                return true;
        }
    }

    return false;
}

CapitalCityTrainerSpellContainer CapitalCityMgr::GetTrainableSpells(Creature* creature)
{
    CapitalCityTrainerSpellContainer container;

    TrainerSpellData const* trainer_spells = creature->GetTrainerSpells();
    if (trainer_spells && !trainer_spells->spellList.empty())
    {
        for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
        {
            TrainerSpell const* tSpell = &itr->second;
            container[tSpell->spell] = tSpell;
        }
    }

    CapitalCityResearchStateConstBounds bound = m_ResearchStateMap.equal_range(creature->GetEntry());
    if (bound.first == bound.second)
        return container;

    for (CapitalCityResearchStateMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.rank > 1 && itr->second.state != CC_RESEARCH_STATE_FINISHED)
        {
            for (int i = 1; i != itr->second.rank; ++i)
            {
                const CapitalCityResearchData* data = GetResearchData(itr->second.researchSet, i);
                if (data)
                {
                    const TrainerSpell* spell = GetTrainerSpell(data->spell);
                    if (spell)
                        container[data->spell] = spell;
                }
            }
        }
        else if (itr->second.state == CC_RESEARCH_STATE_FINISHED)
        {
            const CapitalCityResearchData* data = GetResearchData(itr->second.researchSet, itr->second.rank);
            if (data)
            {
                const TrainerSpell* spell = GetTrainerSpell(data->spell);
                if (spell)
                    container[data->spell] = spell;
            }
        }
    }

    return container;
}

const TrainerSpell* CapitalCityMgr::GetTrainerSpell(uint32 spell)
{
    TrainerSpellMap::const_iterator itr = m_TrainerSpellMap.find(spell);
    if (itr == m_TrainerSpellMap.end())
        return NULL;
    return &itr->second;
}

const VendorItem* CapitalCityMgr::GetVendorItem(uint32 item)
{
    CapitalCityVendorItemMap::const_iterator itr = m_VendorItemMap.find(item);
    if (itr == m_VendorItemMap.end())
        return NULL;
    return &itr->second;
}

CapitalCityVendorItemContainer CapitalCityMgr::GetVendorItems(Creature* creature)
{
    CapitalCityVendorItemContainer container;
    VendorItemData const* items = creature->GetVendorItems();
    if (items)
    {
        for (uint8 slot = 0; slot < items->GetItemCount(); ++slot)
        {
            if (VendorItem const* item = items->GetItem(slot))
                container[item->item] = item;
        }
    }

    CapitalCityResearchStateConstBounds bound = m_ResearchStateMap.equal_range(creature->GetEntry());
    if (bound.first == bound.second)
        return container;

    for (CapitalCityResearchStateMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.rank > 1 && itr->second.state != CC_RESEARCH_STATE_FINISHED)
        {
            for (int i = 1; i != itr->second.rank; ++i)
            {
                const CapitalCityResearchData* data = GetResearchData(itr->second.researchSet, i);
                if (data)
                {
                    const VendorItem* item = GetVendorItem(data->item);
                    if (item)
                        container[data->item] = item;
                }
            }
        }
        else if (itr->second.state == CC_RESEARCH_STATE_FINISHED)
        {
            const CapitalCityResearchData* data = GetResearchData(itr->second.researchSet, itr->second.rank);
            if (data)
            {
                const VendorItem* item = GetVendorItem(data->item);
                if (item)
                    container[data->item] = item;
            }
        }
    }

    return container;
}

bool CapitalCityMgr::HaveAvailableResearch(uint32 entry) const
{
    const CreatureTemplate* proto = sObjectMgr->GetCreatureTemplate(entry);
    if (!proto)
        return false;

    for (uint8 i = 0; i != MAX_CREATURE_RESEARCHSET; ++i)
    {
        const CapitalCityResearchData* data = GetAvailableResearchDataForCreature(entry, proto->ResearchSet[i]);
        if (data)
            return true;
    }

    return false;
}

const CapitalCityNpcResearchState* CapitalCityMgr::GetNpcResearchState(Creature* creature, uint8 index) const
{
    if (!creature)
        return NULL;

    if (index >= MAX_CREATURE_RESEARCHSET)
        return NULL;

    uint32 researchSet = creature->GetCreatureTemplate()->ResearchSet[index];

    return GetNpcResearchState(creature->GetEntry(), researchSet);
}

void CapitalCityMgr::StartResearch(Creature* creature, uint8 index, uint32 announceTeam)
{
    if (!creature)
        return;

    if (index >= MAX_CREATURE_RESEARCHSET)
        return;

    const CreatureTemplate* proto = creature->GetCreatureTemplate();
    if (!proto)
        return;

    uint32 researchSet = proto->ResearchSet[index];

    if (!HaveAllReagentForNextResearch(creature->GetEntry(), researchSet))
        return;

    CapitalCityResearchStateBounds bound = m_ResearchStateMap.equal_range(creature->GetEntry());
    for (CapitalCityResearchStateMap::iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.researchSet == researchSet)
        {
            itr->second.state = CC_RESEARCH_STATE_IN_PROGRESS;
            std::string name = GetResearchSetName(researchSet);
            sWorld->SendGlobalChatMessageToTeam(sObjectMgr->GetServerMessage(34, creature->GetName().c_str(), name.c_str(), itr->second.rank).c_str(), announceTeam);
            return;
        }
    }
}

const CapitalCityResearchData* CapitalCityMgr::GetAvailableResearchDataForCreature(Creature* creature, uint8 index) const
{
    if (!creature)
        return NULL;

    if (index >= MAX_CREATURE_RESEARCHSET)
        return NULL;

    const CreatureTemplate* proto = creature->GetCreatureTemplate();
    if (!proto)
        return NULL;

    uint32 researchSet = proto->ResearchSet[index];

    CapitalCityResearchStateConstBounds bound = m_ResearchStateMap.equal_range(creature->GetEntry());
    for (CapitalCityResearchStateMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.researchSet == researchSet)
            return GetAvailableResearchDataForCreature(creature->GetEntry(), researchSet);
    }

    return GetResearchData(researchSet, 1);
}