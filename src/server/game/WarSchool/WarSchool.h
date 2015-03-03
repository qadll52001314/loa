#ifndef WAR_SCHOOL_H_
#define WAR_SCHOOL_H_

struct WarSchool
{
    std::string Name;
    uint32 ClassMask;
    uint32 TrainerList;
    uint32 Title;
};

typedef std::map<uint32, WarSchool> WarSchoolContainer;
typedef std::multimap<uint32, uint32> WarSchoolSpellContainer;
typedef std::pair<WarSchoolSpellContainer::const_iterator, WarSchoolSpellContainer::const_iterator> WarSchoolSpellBound;
typedef std::vector<uint32> WarSchoolSpellStore;

class WarSchoolMgr
{
public:
    WarSchoolMgr() { }
    ~WarSchoolMgr() { }

    static WarSchoolMgr* instance()
    {
        static WarSchoolMgr instance;
        return &instance;
    }

    void Load();
    std::string GetName(uint32 id) const;
    uint32 GetClassMask(uint32 id) const;
    uint32 GetTrainerList(uint32 id) const;
    bool IsWarSchoolValid(uint32 id) const;
    bool ValidForClass(uint32 id, uint32 classMask) const;
    WarSchoolSpellBound GetWarSchoolSpellBound(uint32 id) const;
    bool GetWarSchoolSpells(uint32 id, WarSchoolSpellStore &store);
    uint32 GetTitle(uint32 id) const;
private:
    WarSchoolContainer _warSchools;
    WarSchoolSpellContainer _warSchoolSpells;
};

#define xWarSchoolMgr WarSchoolMgr::instance()

#endif