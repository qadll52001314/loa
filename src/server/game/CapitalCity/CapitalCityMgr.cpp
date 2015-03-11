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
            city->SetFaction(fields[2].GetUInt32());
			city->SetLevel(fields[3].GetUInt32());
            m_CapitalCities[fields[0].GetUInt32()] = city;
            ++count;
        }
    } while (result->NextRow());

    result = CharacterDatabase.Query("SELECT ID, Resource, MagicPower, Level, UpgradeResource, UpgradeMagicPower FROM capital_city_state");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            if (CapitalCity* city = GetCapitalCityByID(fields[0].GetUInt32()))
            {
                city->SetResource(fields[1].GetUInt32());
				city->SetMagicPower(fields[2].GetUInt32());
                city->SetLevel(fields[3].GetUInt32() > city->GetLevel() ? fields[3].GetUInt32() : city->GetLevel());
                city->SetUpgradeResource(fields[4].GetUInt32());
                city->SetUpgradeMagicPower(fields[5].GetUInt32());
            }
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    m_UpgradeInfo.clear();

    result = WorldDatabase.Query("SELECT Level, Resource, MagicPower FROM capital_city_upgrade_cost");

    count = 0;

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CapitalCityUpgradeCost info;
            info.resource = fields[1].GetUInt32();
            info.magicPower = fields[2].GetUInt32();
            m_UpgradeInfo[fields[0].GetUInt32()] = info;
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City upgrade cost informations", count);

    m_UpgradeSpellSet.clear();

    result = WorldDatabase.Query("SELECT City, SpellSet FROM capital_city_upgrade_spellset");

    count = 0;

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_UpgradeSpellSet.insert(std::pair<uint32, uint32>(fields[0].GetUInt32(), fields[1].GetUInt32()));
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City upgrade spellset defines", count);

    m_UpgradeSpellList.clear();

    result = WorldDatabase.Query("SELECT SpellSet, Rank, Spell, Description, ReqCityRank, ReqItem1, ReqItem2, ReqItem3, ReqItem4, ReqItemCount1, ReqItemCount2, ReqItemCount3, ReqItemCount4 FROM capital_city_upgrade_spellset");

    count = 0;

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CapitalCityUpgradeSpell spell;
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
            m_UpgradeSpellList[fields[0].GetUInt32()] = spell;
            ++count;
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City upgrade spell defines", count);
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

    return city->GetLevel() >= rank;
}

CapitalCity* CapitalCityMgr::FactionBelongsTo(uint32 faction)
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

uint32 CapitalCityMgr::ResourceToNextLevel(uint32 nextLevel)
{
    CapitalCityUpgradeInfoMap::iterator itr = m_UpgradeInfo.find(nextLevel);
    if (itr == m_UpgradeInfo.end())
        return 0;
    return itr->second.resource;
}

uint32 CapitalCityMgr::MagicPowerToNextLevel(uint32 nextLevel)
{
    CapitalCityUpgradeInfoMap::iterator itr = m_UpgradeInfo.find(nextLevel);
    if (itr == m_UpgradeInfo.end())
        return 0;
    return itr->second.magicPower;
}
