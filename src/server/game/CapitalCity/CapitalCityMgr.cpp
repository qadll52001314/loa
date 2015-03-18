#include "CapitalCityMgr.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"

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

    m_SpellSet.clear();

    count = 0;

    result = WorldDatabase.Query("SELECT ID, Name FROM capital_city_spellset");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_SpellSet[fields[0].GetUInt32()] = fields[1].GetString();
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City spellsets", count);

    m_ResearchSpellSet.clear();

    result = WorldDatabase.Query("SELECT SpellSet, Rank, Spell, Description, ReqCityRank, ReqItem1, ReqItem2, ReqItem3, ReqItem4, ReqItemCount1, ReqItemCount2, ReqItemCount3, ReqItemCount4, Progress FROM capital_city_upgrade_spells");

    count = 0;

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            CapitalCityResearchSpell spell;
            spell.rank = fields[1].GetUInt32();
            spell.spell = fields[2].GetUInt32();
            spell.description = fields[3].GetUInt32();
            spell.reqCityRank = fields[4].GetUInt32();
            spell.reqItem1 = fields[5].GetUInt32();
            spell.reqItem2 = fields[6].GetUInt32();
            spell.reqItem3 = fields[7].GetUInt32();
            spell.reqItem4 = fields[8].GetUInt32();
            spell.reqItemCount1 = fields[9].GetUInt32();
            spell.reqItemCount2 = fields[10].GetUInt32();
            spell.reqItemCount3 = fields[11].GetUInt32();
            spell.reqItemCount4 = fields[12].GetUInt32();
            spell.progress = fields[13].GetUInt32();
            m_ResearchSpellSet.insert(std::pair<uint32, CapitalCityResearchSpell>(fields[0].GetUInt32(), spell));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City upgrade spell defines", count);

    m_ResearcherSpellSet.clear();

    result = WorldDatabase.Query("SELECT ResearcherEntry, SpellSet FROM capital_city_upgrade_research_info");

    count = 0;

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_ResearcherSpellSet.insert(std::pair<uint32, uint32>(fields[0].GetUInt32(), fields[1].GetUInt32()));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City upgrade researcher defines", count);

    count = 0;

    result = CharacterDatabase.Query("SELECT Entry, SpellSet, Rank, State, Progress, ItemCount1, ItemCount2, ItemCount3, ItemCount4 FROM capital_city_npc_research_state");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CapitalCityNpcResearchState state;
            state.spellSet = fields[1].GetUInt32();
            state.rank = fields[2].GetUInt32();
            state.state = fields[3].GetUInt8();
            state.progress = fields[4].GetUInt8();
            state.itemCount1 = fields[5].GetUInt32();
            state.itemCount2 = fields[6].GetUInt32();
            state.itemCount3 = fields[7].GetUInt32();
            state.itemCount4 = fields[8].GetUInt32();
            if (state.state >= CC_RESEARCH_STATE_MAX)
                state.state = CC_RESEARCH_STATE_NOT_STARTED;
            m_ResearchStateSet.insert(std::pair<uint32, CapitalCityNpcResearchState>(fields[0].GetUInt32(), state));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City research states", count);

    count = 0;

    result = WorldDatabase.Query("SELECT City, SpellSet FROM capital_city_learnable_spells");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_LearnableSpellSet.insert(std::pair<uint32, uint32>(fields[0].GetUInt32(), fields[1].GetUInt32()));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City learnable spellset defines", count);
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

bool CapitalCityMgr::ReachedRequiredRank(Creature* creature, uint32 rank) const
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

CapitalCityResearchSpellSetContainer CapitalCityMgr::GetAvailableResearch(Creature* researcher) const
{
    CapitalCityResearchSpellSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    uint32 entry = researcher->GetEntry();

    CapitalCityResearchSpellSetList list = GetResearchListForCreatureEntry(entry);
    if (list.empty())
        return container;

    for (int i = 0; i != list.size(); ++i)
    {
        const CapitalCityResearchSpell* spell = GetNextAvailableResearchSpellForCreature(entry, list[i]);
        if (spell)
            container.push_back(spell);
    }

    return container;
}

CapitalCityResearchSpellSetContainer CapitalCityMgr::GetInProgressResearch(Creature* researcher) const
{
    CapitalCityResearchSpellSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    uint32 entry = researcher->GetCreatureTemplate()->Entry;

    CapitalCityResearchSpellSetList list = GetResearchListForCreatureEntry(entry);
    if (list.empty())
        return container;

    for (int i = 0; i != list.size(); ++i)
    {
        const CapitalCityResearchSpell* spell = GetInProgressResearchSpellForCreature(entry, list[i]);
        if (spell)
            container.push_back(spell);
    }

    return container;
}

CapitalCityResearchSpellSetList CapitalCityMgr::GetResearchListForCreatureEntry(uint32 entry) const
{
    CapitalCityResearchSpellSetList list;
    CapitalCityResearcherSpellSetConstBounds bound = m_ResearcherSpellSet.equal_range(entry);
    if (bound.first == bound.second)
        return list;

    for (CapitalCityResearcherSpellSetMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        list.push_back(itr->second);

    return list;
}

uint32 CapitalCityMgr::GetResearchSpellRank(uint32 spellSet, uint32 spell) const
{
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellSet);
    if (bound.first == bound.second)
        return 0;

    for (CapitalCityResearchSpellMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spell == spell)
            return itr->second.rank;
    }

    return 0;
}

PCapitalCityResearchSpellMap CapitalCityMgr::GetSpellSet(uint32 spellset) const
{
    PCapitalCityResearchSpellMap set;
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellset);
    if (bound.first == bound.second)
        return set;

    for (CapitalCityResearchSpellMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        set.insert(std::pair<uint32, const CapitalCityResearchSpell*>(itr->second.rank, &itr->second));

    return set;
}

const CapitalCityResearchSpell* CapitalCityMgr::GetNextAvailableResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet) const
{
    if (!CanCreatureResearch(creatureEntry, spellSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, spellSet);
    if (!state.spell)
        return GetResearchSpell(spellSet, 1);

    if (state.state == CC_RESEARCH_STATE_FINISHED)
    {
        const CapitalCityResearchSpell* spell = GetResearchSpell(spellSet, state.spell->rank + 1);
        return spell; // maybe null
    }

    return NULL;
}

const CapitalCityResearchSpell* CapitalCityMgr::GetInProgressResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet) const
{
    if (!CanCreatureResearch(creatureEntry, spellSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, spellSet);
    if (!state.spell)
        return NULL;

    if (state.state == CC_RESEARCH_STATE_IN_PROGRESS)
    {
        const CapitalCityResearchSpell* spell = GetResearchSpell(spellSet, state.spell->rank + 1);
        return spell; // maybe null
    }

    return NULL;
}

const CapitalCityResearchSpell* CapitalCityMgr::GetCompletedResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet) const
{
    if (!CanCreatureResearch(creatureEntry, spellSet))
        return NULL;

    CapitalCityResearchState state = GetResearchState(creatureEntry, spellSet);
    if (!state.spell)
        return NULL;

    if (state.state == CC_RESEARCH_STATE_FINISHED)
    {
        const CapitalCityResearchSpell* spell = GetResearchSpell(spellSet, state.spell->rank + 1);
        return spell; // maybe null
    }

    return NULL;
}

CapitalCityResearchState CapitalCityMgr::GetResearchState(uint32 entry, uint32 spellSet) const
{
    CapitalCityResearchState state;
    state.spell = NULL;
    state.state = CC_RESEARCH_STATE_NOT_STARTED;
    CapitalCityResearcherSpellSetConstBounds bound = m_ResearcherSpellSet.equal_range(entry);
    if (bound.first == bound.second)
        return state;

    CapitalCityResearchStateConstBounds stateBound = m_ResearchStateSet.equal_range(entry);
    if (stateBound.first == stateBound.second)
    {
        // not started
        state.spell = GetResearchSpell(spellSet, 1);
    }
    else
    {
        for (CapitalCityResearchStateMap::const_iterator itr = stateBound.first; itr != stateBound.second; ++itr)
        {
            if (itr->second.spellSet == spellSet)
            {
                state.state = itr->second.state;
                state.spell = GetResearchSpell(spellSet, itr->second.rank);
            }
        }
    }

    return state;
}

bool CapitalCityMgr::CanCreatureResearch(uint32 creatureEntry, uint32 spellSet) const
{
    CapitalCityResearchSpellSetList list = GetResearchListForCreatureEntry(creatureEntry);
    if (list.empty())
        return false;

    CapitalCityResearchSpellSetList::const_iterator itr = std::find(list.begin(), list.end(), spellSet);
    if (itr != list.end())
        return true;
    return false;
}

CapitalCityResearchSpellSetContainer CapitalCityMgr::GetResearchCompleted(Creature* researcher) const
{
    CapitalCityResearchSpellSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    uint32 entry = researcher->GetCreatureTemplate()->Entry;

    CapitalCityResearchSpellSetList list = GetResearchListForCreatureEntry(entry);
    if (list.empty())
        return container;

    for (int i = 0; i != list.size(); ++i)
    {
        const CapitalCityResearchSpell* spell = GetCompletedResearchSpellForCreature(entry, list[i]);
        if (spell)
            container.push_back(spell);
    }

    return container;
}

void CapitalCityMgr::UpdateResearchState(uint32 entry, uint32 spellSet, uint32 rank, uint8 state)
{
    CapitalCityResearchStateBounds bound = m_ResearchStateSet.equal_range(entry);
    if (bound.first == bound.second)
        return;

    for (CapitalCityResearchStateMap::iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spellSet == spellSet)
        {
            itr->second.rank = rank;
            itr->second.state = state;
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CAPITAL_CITY_RESEARCH_STATE);
            stmt->setUInt32(0, entry);
            stmt->setUInt32(1, spellSet);
            stmt->setUInt32(2, rank);
            stmt->setUInt8(3, state);
            CharacterDatabase.Execute(stmt);
            return;
        }
    }

    // no result found, create one.
    CapitalCityNpcResearchState newState;
    newState.spellSet = spellSet;
    newState.rank = rank;
    newState.state = state;
    m_ResearchStateSet.insert(std::pair<uint32, CapitalCityNpcResearchState>(entry, newState));

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CAPITAL_CITY_RESEARCH_STATE);
    stmt->setUInt32(0, entry);
    stmt->setUInt32(1, spellSet);
    stmt->setUInt32(2, rank);
    stmt->setUInt8(3, state);
    CharacterDatabase.Execute(stmt);
}

const CapitalCityResearchSpell* CapitalCityMgr::GetResearchSpell(uint32 spellSet, uint32 rank) const
{
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellSet);
    if (bound.first == bound.second)
        return NULL;

    for (CapitalCityResearchSpellMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
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
        if (itr->second->GetTeam() == team)
            v.push_back(itr->second);
    }
    return v;
}

CapitalCitySpellList CapitalCityMgr::GetLearnableSpellsForTeam(uint32 team) const
{
    CapitalCitySpellList v;

    CapitalCityList list = GetCapitalCitiesForTeam(team);

    for (int i = 0; i != list.size(); ++i)
    {
        CapitalCity* city = list[i];

        CapitalCityLearnableSpellConstBounds bound = m_LearnableSpellSet.equal_range(city->GetID());
        if (bound.first == bound.second)
            continue;

        for (CapitalCityLearnableSpellSetMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            PCapitalCityResearchSpellMap spells = GetSpellSet(itr->second);
            if (!spells.empty())
            {
                uint32 maxRank = 0;
                uint32 maxSpell = 0;

                for (PCapitalCityResearchSpellMap::const_iterator itrSpell = spells.begin(); itrSpell != spells.end(); ++itrSpell)
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

CapitalCitySpellList CapitalCityMgr::GetSpellsForRank(uint32 cityID, uint32 rank) const
{
    CapitalCitySpellList list;
    CapitalCity* city = GetCapitalCityByID(cityID);
    if (!city)
        return list;

    CapitalCityLearnableSpellConstBounds bound = m_LearnableSpellSet.equal_range(cityID);
    if (bound.first == bound.second)
        return list;

    for (CapitalCityLearnableSpellSetMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        PCapitalCityResearchSpellMap set = GetSpellSet(itr->second);
        if (set.empty())
            continue;

        for (PCapitalCityResearchSpellMap::const_iterator itrSet = set.begin(); itrSet != set.end(); ++itrSet)
        {
            if (itrSet->second->reqCityRank == rank)
                list.push_back(itrSet->second->spell);
        }
    }

    return list;
}

void CapitalCityMgr::SendResearchProgress(Player* receiver, uint32 researcher, uint32 spellSet) const
{
    const CapitalCityNpcResearchState* state = GetNpcResearchState(researcher, spellSet);

    const CapitalCityResearchSpell* spell = GetCapitalCityResearchSpell(spellSet, state ? state->rank : 1);
    if (!spell)
        return;

    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_PROGRESS, state ? state->progress * 100 / spell->progress : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_RANK, state ? state->rank : 1);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM1_COUNT, state ? state->itemCount1 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM2_COUNT, state ? state->itemCount2 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM3_COUNT, state ? state->itemCount3 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM4_COUNT, state ? state->itemCount4 : 0);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM1_MAX, spell->reqItemCount1);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM2_MAX, spell->reqItemCount2);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM3_MAX, spell->reqItemCount3);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM4_MAX, spell->reqItemCount4);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_REQ_CITY_RANK, spell->reqCityRank);
}

uint32 CapitalCityMgr::GetResearchDescriptionText(uint32 spellSet, uint32 spell) const
{
    PCapitalCityResearchSpellMap set = GetSpellSet(spellSet);
    if (set.empty())
        return 0;

    for (PCapitalCityResearchSpellMap::const_iterator itr = set.begin(); itr != set.end(); ++itr)
    {
        if (itr->second->spell == spell)
            return itr->second->description;
    }

    return 0;
}

const CapitalCityNpcResearchState* CapitalCityMgr::GetNpcResearchState(uint32 researcher, uint32 spellSet) const
{
    CapitalCityResearchStateConstBounds bound = m_ResearchStateSet.equal_range(researcher);
    if (bound.first != bound.second)
    {
        for (CapitalCityResearchStateMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.spellSet == spellSet)
                return &itr->second;
        }
    }
    return NULL;
}

const CapitalCityResearchSpell* CapitalCityMgr::GetCapitalCityResearchSpell(uint32 spellSet, uint32 rank) const
{
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellSet);
    if (bound.first != bound.second)
    {
        for (CapitalCityResearchSpellMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.rank == rank)
                return &itr->second;
        }
    }
    return NULL;
}

std::string CapitalCityMgr::GetSpellSetName(uint32 spellSet) const
{
    CapitalCitySpellSetMap::const_iterator itr = m_SpellSet.find(spellSet);
    if (itr != m_SpellSet.end())
        return itr->second;
    return "";
}

bool CapitalCityMgr::HaveAllReagentForNextResearch(uint32 researcherEntry, uint32 spellSet) const
{
    const CapitalCityResearchSpell* spell = GetNextAvailableResearchSpellForCreature(researcherEntry, spellSet);
    if (!spell)
        return false;

    const CapitalCityNpcResearchState* state = GetNpcResearchState(researcherEntry, spellSet);
    if (!state)
        return false;

    return state->itemCount1 >= spell->reqItemCount1
        && state->itemCount2 >= spell->reqItemCount2
        && state->itemCount3 >= spell->reqItemCount3
        && state->itemCount4 >= spell->reqItemCount4;
}

void CapitalCityMgr::StartNextAvailableResearch(uint32 researcherEntry, uint32 spellSet)
{
    if (!HaveAllReagentForNextResearch(researcherEntry, spellSet))
        return;

    CapitalCityResearchStateBounds bound = m_ResearchStateSet.equal_range(researcherEntry);
    if (bound.first == bound.second) // shouldnt happen
        return;

    for (CapitalCityResearchStateMap::iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spellSet == spellSet && itr->second.state == CC_RESEARCH_STATE_NOT_STARTED)
        {
            UpdateResearchState(researcherEntry, spellSet, /**/, CC_RESEARCH_STATE_IN_PROGRESS);
            return;
        }
    }
}

void CapitalCityMgr::ResearchUpdate()
{
    // this method will be called every 1 minute, so just +1 to all research in progress.
    for (CapitalCityResearchStateMap::iterator itr = m_ResearchStateSet.begin(); itr != m_ResearchStateSet.end(); ++itr)
    {
        if (itr->second.state == CC_RESEARCH_STATE_IN_PROGRESS)
        {
            itr->second.progress += 1;

            const CapitalCityResearchSpell* spell = GetCapitalCityResearchSpell(itr->second.spellSet, itr->second.rank);

            ASSERT(spell);

            if (itr->second.progress >= spell->progress)
                itr->second.state = CC_RESEARCH_STATE_FINISHED;
        }
    }
}

void CapitalCityMgr::AddReagentTo(uint32 researcherEntry, uint32 spellSet, uint32 item, uint32 count)
{
    const CapitalCityResearchSpell* spell = GetNextAvailableResearchSpellForCreature(researcherEntry, spellSet);
    if (!spell)
        return;

    CapitalCityResearchStateBounds bound = m_ResearchStateSet.equal_range(researcherEntry);
    if (bound.first == bound.second)
    {
        CapitalCityNpcResearchState state = BuildNewResearchState(spell, spellSet, item, count);
        m_ResearchStateSet.insert(std::pair<uint32, CapitalCityNpcResearchState>(researcherEntry, state));
    }
    else
    {
        for (CapitalCityResearchStateMap::iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.spellSet == spellSet)
            {
                if (spell->reqItem1 == item)
                    itr->second.itemCount1 += count;
                else if (spell->reqItem2 == item)
                    itr->second.itemCount2 += count;
                else if (spell->reqItem3 == item)
                    itr->second.itemCount3 += count;
                else if (spell->reqItem4 == item)
                    itr->second.itemCount4 += count;
                return;
            }
        }

        CapitalCityNpcResearchState state = BuildNewResearchState(spell, spellSet, item, count);
        m_ResearchStateSet.insert(std::pair<uint32, CapitalCityNpcResearchState>(researcherEntry, state));
    }
}

CapitalCityNpcResearchState CapitalCityMgr::BuildNewResearchState(const CapitalCityResearchSpell* spell, uint32 spellSet, uint32 item, uint32 count) const
{
    ASSERT(spell);

    CapitalCityNpcResearchState state;
    state.itemCount1 = 0;
    state.itemCount2 = 0;
    state.itemCount3 = 0;
    state.itemCount4 = 0;
    state.progress = 0;
    state.rank = 1;
    state.spellSet = spellSet;
    state.state = CC_RESEARCH_STATE_NOT_STARTED;

    if (spell->reqItem1 == item)
        state.itemCount1 += count;
    else if (spell->reqItem2 == item)
        state.itemCount2 += count;
    else if (spell->reqItem3 == item)
        state.itemCount3 += count;
    else if (spell->reqItem4 == item)
        state.itemCount4 += count;

    return state;
}
