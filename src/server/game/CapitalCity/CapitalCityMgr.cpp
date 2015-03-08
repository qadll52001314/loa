#include "CapitalCityMgr.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"

void CapitalCityMgr::LoadCapitalCities()
{
    m_CapitalCities.clear();

    uint32 oldMSTime = getMSTime();


    QueryResult result = WorldDatabase.Query("SELECT zone, faction, startlevel, scriptname FROM capital_cities");

    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 Capital City definitions. DB table `capital_cities` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 scriptId = sObjectMgr->GetScriptId(fields[3].GetCString());
        CapitalCity* city = sScriptMgr->CreateCapitalCity(scriptId);
        if (city)
        {
            city->SetFaction(fields[1].GetUInt32());
			city->SetLevel(fields[2].GetUInt32());
            city->SetZone(fields[0].GetUInt32());
            m_CapitalCities[fields[0].GetUInt32()] = city;
            ++count;
        }
    } while (result->NextRow());

    result = CharacterDatabase.Query("SELECT zone, resource, magic, level FROM capital_city_state");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            if (CapitalCity* city = GetCapitalCity(fields[0].GetUInt32()))
            {
                city->SetResource(fields[1].GetUInt32());
				city->SetMagicPower(fields[2].GetUInt32());
                city->SetLevel(fields[3].GetUInt32() > city->GetLevel() ? fields[3].GetUInt32() : city->GetLevel());
            }
        } while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u Capital City definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
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
    if (CapitalCity* city = GetCapitalCity(zone))
        city->HandlePlayerEnter(player);
}

void CapitalCityMgr::HandlePlayerLeave(Player* player, uint32 zone)
{
    if (CapitalCity* city = GetCapitalCity(zone))
        city->HandlePlayerLeave(player);
}

CapitalCity* CapitalCityMgr::GetCapitalCity(uint32 zone) const
{
    CapitalCityMap::const_iterator itr = m_CapitalCities.find(zone);
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

    CapitalCity* city = FactionBelongsTo(creature->GetCreatureTemplate()->faction);

    if (!city)
        return false;

    return city->GetLevel() >= rank;
}

CapitalCity* CapitalCityMgr::FactionBelongsTo(uint32 faction)
{
    switch (faction)
    {
    case 29:
    case 126: // darkspear troll
        return GetCapitalCity(CC_ORGRIMMAR);
    case 104:
    case 105:
        return GetCapitalCity(CC_THUNDERBLUFF);
    case 68:
    case 71:
        return GetCapitalCity(CC_UNDERCITY);
    case 1603:
    case 1604:
        return GetCapitalCity(CC_SILVERMOON);
    case 12:
    case 11:
        return GetCapitalCity(CC_STORMWIND);
    case 55:
    case 57:
        return GetCapitalCity(CC_IRONFORGE);
    case 80:
    case 79:
        return GetCapitalCity(CC_DARNASSUS);
    case 1638:
    case 1639:
        return GetCapitalCity(CC_EXODAR);
    default:
        return NULL;
    }
}