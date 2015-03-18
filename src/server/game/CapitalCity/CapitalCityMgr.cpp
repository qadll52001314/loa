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

    result = WorldDatabase.Query("SELECT SpellSet, Rank, Spell, Description, ReqCityRank, ReqItem1, ReqItem2, ReqItem3, ReqItem4, ReqItemCount1, ReqItemCount2, ReqItemCount3, ReqItemCount4 FROM capital_city_upgrade_spells");

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

void CapitalCityMgr::SendStateTo(Player* player, uint32 index, uint32 value)
{
    player->SendUpdateWorldState(index, value);
}

void CapitalCityMgr::ClearStateOf(Player* player, uint32 index)
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

bool CapitalCityMgr::ReachedRequiredRank(Creature* creature, uint32 rank)
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

CapitalCityResearchSpellSetContainer CapitalCityMgr::GetResearchAvailable(Creature* researcher)
{
    CapitalCityResearchSpellSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    uint32 entry = researcher->GetCreatureTemplate()->Entry;

    CapitalCitySpellResearchSetList list = GetResearchListForCreatureEntry(entry);
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

CapitalCityResearchSpellSetContainer CapitalCityMgr::GetResearchInProgress(Creature* researcher)
{
    CapitalCityResearchSpellSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    uint32 entry = researcher->GetCreatureTemplate()->Entry;

    CapitalCitySpellResearchSetList list = GetResearchListForCreatureEntry(entry);
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

CapitalCitySpellResearchSetList CapitalCityMgr::GetResearchListForCreatureEntry(uint32 entry)
{
    CapitalCitySpellResearchSetList list;
    CapitalCityResearcherSpellSetConstBounds bound = m_ResearcherSpellSet.equal_range(entry);
    if (bound.first == bound.second)
        return list;

    for (CapitalCityResearcherSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
        list.push_back(itr->second);

    return list;
}

uint32 CapitalCityMgr::GetResearchSpellRank(uint32 spellSet, uint32 spell) const
{
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellSet);
    if (bound.first == bound.second)
        return 0;

    for (CapitalCityResearchSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spell == spell)
            return itr->second.rank;
    }

    return 0;
}

CapitalCityResearchSpellSet CapitalCityMgr::GetSpellSet(uint32 spellset)
{
    CapitalCityResearchSpellSet set;
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellset);
    if (bound.first == bound.second)
        return set;

    for (CapitalCityResearchSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
        set.insert(std::pair<uint32, const CapitalCityResearchSpell*>(itr->second.rank, &itr->second));

    return set;
}

const CapitalCityResearchSpell* CapitalCityMgr::GetNextAvailableResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet)
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

const CapitalCityResearchSpell* CapitalCityMgr::GetInProgressResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet)
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

const CapitalCityResearchSpell* CapitalCityMgr::GetCompletedResearchSpellForCreature(uint32 creatureEntry, uint32 spellSet)
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

CapitalCityResearchState CapitalCityMgr::GetResearchState(uint32 entry, uint32 spellSet)
{
    CapitalCityResearchState state;
    CapitalCityResearchStateConstBounds bound = m_ResearchStateSet.equal_range(entry);
    if (bound.first == bound.second)
        return state;

    for (CapitalCityResearchStateSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spellSet == spellSet)
        {
            const CapitalCityResearchSpell* sp = GetResearchSpell(spellSet, itr->second.rank);
            if (!sp)
                return state;
            state.spell = sp;
            state.state = itr->second.state;
            return state;
        }
    }

    return state;
}

bool CapitalCityMgr::CanCreatureResearch(uint32 creatureEntry, uint32 spellSet)
{
    CapitalCitySpellResearchSetList list = GetResearchListForCreatureEntry(creatureEntry);
    if (list.empty())
        return false;

    CapitalCitySpellResearchSetList::const_iterator itr = std::find(list.begin(), list.end(), spellSet);
    if (itr != list.end())
        return true;
    return false;
}

CapitalCityResearchSpellSetContainer CapitalCityMgr::GetResearchCompleted(Creature* researcher)
{
    CapitalCityResearchSpellSetContainer container;

    CapitalCity* city = researcher->GetCapitalCity();

    if (!city)
        return container;

    uint32 entry = researcher->GetCreatureTemplate()->Entry;

    CapitalCitySpellResearchSetList list = GetResearchListForCreatureEntry(entry);
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

    for (CapitalCityResearchStateSet::iterator itr = bound.first; itr != bound.second; ++itr)
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

const CapitalCityResearchSpell* CapitalCityMgr::GetResearchSpell(uint32 spellSet, uint32 rank)
{
    CapitalCityResearchSpellConstBounds bound = m_ResearchSpellSet.equal_range(spellSet);
    if (bound.first == bound.second)
        return NULL;

    for (CapitalCityResearchSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.rank == rank)
            return &itr->second;
    }

    return NULL;
}

CapitalCityList CapitalCityMgr::GetCapitalCitiesForTeam(uint32 team)
{
    CapitalCityList v;
    for (CapitalCityMap::const_iterator itr = m_CapitalCities.begin(); itr != m_CapitalCities.end(); ++itr)
    {
        if (itr->second->GetTeam() == team)
            v.push_back(itr->second);
    }
    return v;
}

CapitalCitySpellList CapitalCityMgr::GetLearnableSpellsForTeam(uint32 team)
{
    CapitalCitySpellList v;

    CapitalCityList list = GetCapitalCitiesForTeam(team);

    for (int i = 0; i != list.size(); ++i)
    {
        CapitalCity* city = list[i];

        CapitalCityLearnableSpellConstBounds bound = m_LearnableSpellSet.equal_range(city->GetID());
        if (bound.first == bound.second)
            continue;

        for (CapitalCityLearnableSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            CapitalCityResearchSpellSet spells = GetSpellSet(itr->second);
            if (!spells.empty())
            {
                uint32 maxRank = 0;
                uint32 maxSpell = 0;

                for (CapitalCityResearchSpellSet::const_iterator itrSpell = spells.begin(); itrSpell != spells.end(); ++itrSpell)
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

CapitalCitySpellList CapitalCityMgr::GetSpellsForRank(uint32 cityID, uint32 rank)
{
    CapitalCitySpellList list;
    CapitalCity* city = GetCapitalCityByID(cityID);
    if (!city)
        return list;

    CapitalCityLearnableSpellConstBounds bound = m_LearnableSpellSet.equal_range(cityID);
    if (bound.first == bound.second)
        return list;

    for (CapitalCityLearnableSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        CapitalCityResearchSpellSet set = GetSpellSet(itr->second);
        if (set.empty())
            continue;

        for (CapitalCityResearchSpellSet::const_iterator itrSet = set.begin(); itrSet != set.end(); ++itrSet)
        {
            if (itrSet->second->reqCityRank == rank)
                list.push_back(itrSet->second->spell);
        }
    }

    return list;
}

void CapitalCityMgr::SendResearchProgress(Player* receiver, uint32 researcher, uint32 spellSet)
{
    const CapitalCityNpcResearchState* state = GetNpcResearchState(researcher, spellSet);
    if (!state)
        return;

    const CapitalCityResearchSpell* spell = GetCapitalCityResearchSpell(spellSet, state->rank);
    if (!spell)
        return;

    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_PROGRESS, state->progress);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_RANK, state->rank);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM1_COUNT, state->itemCount1);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM2_COUNT, state->itemCount2);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM3_COUNT, state->itemCount3);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM4_COUNT, state->itemCount4);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM1_MAX, spell->reqItemCount1);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM2_MAX, spell->reqItemCount2);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM3_MAX, spell->reqItemCount3);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_ITEM4_MAX, spell->reqItemCount4);
    receiver->SendUpdateWorldState(WORLDSTATE_RESEARCH_REQ_CITY_RANK, spell->reqCityRank);
}

uint32 CapitalCityMgr::GetResearchDescriptionText(uint32 spellSet, uint32 spell)
{
    CapitalCityResearchSpellSet set = GetSpellSet(spellSet);
    if (set.empty())
        return 0;

    for (CapitalCityResearchSpellSet::const_iterator itr = set.begin(); itr != set.end(); ++itr)
    {
        if (itr->second->spell == spell)
            return itr->second->description;
    }

    return 0;
}

const CapitalCityNpcResearchState* CapitalCityMgr::GetNpcResearchState(uint32 researcher, uint32 spellSet)
{
    CapitalCityResearchStateConstBounds bound = m_ResearchStateSet.equal_range(researcher);
    if (bound.first != bound.second)
    {
        for (CapitalCityResearchStateSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
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
        for (CapitalCityResearchSpellSet::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.rank == rank)
                return &itr->second;
        }
    }
    return NULL;
}

std::string CapitalCityMgr::GetSpellSetName(uint32 spellSet)
{
    CapitalCitySpellSet::const_iterator itr = m_SpellSet.find(spellSet);
    if (itr != m_SpellSet.end())
        return itr->second;
    return "";
}

bool CapitalCityMgr::HaveAllReagentForNextResearch(uint32 researcherEntry, uint32 spellSet)
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

    for (CapitalCityResearchStateSet::iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.spellSet == spellSet && itr->second.state == CC_RESEARCH_STATE_NOT_STARTED)
        {
            UpdateResearchState(researcherEntry, spellSet, 0, CC_RESEARCH_STATE_IN_PROGRESS);
            return; // or this invalidate the iterator
        }
    }
}