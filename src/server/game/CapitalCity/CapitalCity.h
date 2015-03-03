#ifndef CAPITAL_CITY_H_
#define CAPITAL_CITY_H_

#define RESOURCE_GATHER_DIFF                900000 // 15min
#define WORLDSTATE_IN_CAPITAL_CITY          6005
#define WORLDSTATE_CAPITAL_CITY_LEVEL       6003
#define WORLDSTATE_CAPITAL_CITY_RESOURCE    6004
#define WORLDSTATE_CAPITAL_CITY_MAGICPOWER  6006

class CapitalCity
{
public:
	CapitalCity() { m_Zone = 0; m_Faction = 0; m_Level = 1; m_Resource = 0; m_MagicPower = 0; }
    ~CapitalCity() { }

    void Update(uint32 diff);
    void SetResource(uint32 resource) { m_Resource = resource; }
    void SetLevel(uint32 level) { m_Level = level; }
    uint32 GetLevel() { return m_Level; }
    uint32 GetResource() { return m_Resource; }
	void SetMagicPower(uint32 magic) { m_MagicPower = magic; }
	uint32 GetMagicPower() { return m_MagicPower; }
    void SetFaction(uint32 faction) { m_Faction = faction; }
    uint32 GetFaction() { return m_Faction; }
    void SetZone(uint32 zone) { m_Zone = zone; }
    void AddResource(int32 resource, Player* player = NULL);
    void AddMagicPower(int32 magic, Player* player = NULL);
    void LevelUp();
    std::string GetName();

    void Save();

    virtual void GatherResource() { }

    void HandlePlayerEnter(Player*);
    void HandlePlayerLeave(Player*);

    void SendStateUpdate();
    void ClearState(uint32 index);
    void SendAllState(Player*);
    void ClearAllState(Player*);
private:
    uint32 m_Zone;
    uint32 m_Faction;
	uint32 m_Level;
    uint32 m_Resource;
	uint32 m_MagicPower;
};

#endif
