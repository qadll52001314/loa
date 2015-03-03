#include "WarSchool.h"

void WarSchoolMgr::Load()
{
    _warSchools.clear();
    QueryResult result = WorldDatabase.Query("SELECT id, name, class, trainer_list, title_id FROM war_school");
    if (result)
    {
        uint32 count = 0;
        do
        {
            ++count;
            Field* fields = result->Fetch();
            WarSchool school;
            school.Name = fields[1].GetString();
            school.ClassMask = fields[2].GetUInt32();
            school.TrainerList = fields[3].GetUInt32();
            school.Title = fields[4].GetUInt32();
            _warSchools[fields[0].GetUInt32()] = school;
        } while (result->NextRow());

        TC_LOG_INFO("server.loading", ">> Loaded %u War School Define.", count);
    }

    _warSchoolSpells.clear();
    result = WorldDatabase.Query("SELECT entry, spell FROM npc_trainer WHERE entry > 200000");
    if (result)
    {
        uint32 count = 0;
        do 
        {
            ++count;
            Field* fields = result->Fetch();
            _warSchoolSpells.insert({ fields[0].GetUInt32(), fields[1].GetUInt32() });
        } while (result->NextRow());
    }

}

std::string WarSchoolMgr::GetName(uint32 id) const
{
    WarSchoolContainer::const_iterator itr = _warSchools.find(id);
    if (itr == _warSchools.end())
        return "Undefined";
    return itr->second.Name;
}

uint32 WarSchoolMgr::GetClassMask(uint32 id) const
{
    WarSchoolContainer::const_iterator itr = _warSchools.find(id);
    if (itr == _warSchools.end())
        return 0;
    return itr->second.ClassMask;
}

uint32 WarSchoolMgr::GetTrainerList(uint32 id) const
{
    WarSchoolContainer::const_iterator itr = _warSchools.find(id);
    if (itr == _warSchools.end())
        return 0;
    return itr->second.TrainerList;
}

bool WarSchoolMgr::IsWarSchoolValid(uint32 id) const
{
    WarSchoolContainer::const_iterator itr = _warSchools.find(id);
    if (itr == _warSchools.end())
        return false;
    return true;
}

bool WarSchoolMgr::ValidForClass(uint32 id, uint32 classMask) const
{
    if (GetClassMask(id) & classMask)
        return true;
    return false;
}

WarSchoolSpellBound WarSchoolMgr::GetWarSchoolSpellBound(uint32 id) const
{
    return _warSchoolSpells.equal_range(id);
}

bool WarSchoolMgr::GetWarSchoolSpells(uint32 id, WarSchoolSpellStore &store)
{
    if (!IsWarSchoolValid(id))
        return false;
    WarSchoolSpellBound bound = GetWarSchoolSpellBound(id);
    if (bound.first != bound.second)
    {
        for (WarSchoolSpellContainer::const_iterator itr = bound.first; itr != bound.second; ++itr)
            store.push_back(itr->second);
        return true;
    }
    return false;
}

uint32 WarSchoolMgr::GetTitle(uint32 id) const
{
    WarSchoolContainer::const_iterator itr = _warSchools.find(id);
    if (itr == _warSchools.end())
        return 0;
    return itr->second.Title;
}