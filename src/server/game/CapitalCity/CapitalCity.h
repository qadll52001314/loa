#ifndef CAPITAL_CITY_H_
#define CAPITAL_CITY_H_

#include "Common.h"
#include "SharedDefines.h"
#include "Player.h"

#define WORLDSTATE_IN_CAPITAL_CITY                  6005
#define WORLDSTATE_CAPITAL_CITY_LEVEL               6003
#define WORLDSTATE_CAPITAL_CITY_RESOURCE            6004
#define WORLDSTATE_CAPITAL_CITY_MAGICPOWER          6006
#define WORLDSTATE_CC_UPGRADE_RESOURCE_PROGRESS     6011
#define WORLDSTATE_CC_UPGRADE_MAGICPOWER_PROGRESS   6012
#define WORLDSTATE_CC_UPGRADING                     6013

class CapitalCity
{
public:
    CapitalCity() { m_Zone = 0; m_Faction = 0; m_Level = 1; m_Resource = 0; m_MagicPower = 0; }
    ~CapitalCity() { }

    void Update();
    void SetResource(uint32 resource) { m_Resource = resource; }
    void SetLevel(uint32 level) { m_Level = level; }
    uint32 GetLevel() { return m_Level; }
    uint32 GetResource() { return m_Resource; }
	void SetMagicPower(uint32 magic) { m_MagicPower = magic; }
	uint32 GetMagicPower() { return m_MagicPower; }
    void SetUpgradeResource(uint32 resource) { m_UpgradeResource = resource; }
    uint32 GetUpgradeResource() { return m_UpgradeResource; }
    void SetUpgradeMagicPower(uint32 magic) { m_UpgradeMagicPower = magic; }
    uint32 GetUpgradeMagicPower() { return m_UpgradeMagicPower; }
    void SetFaction(uint32 faction) { m_Faction = faction; }
    uint32 GetFaction() { return m_Faction; }
    void SetZone(uint32 zone) { m_Zone = zone; }
    uint32 GetZone() { return m_Zone; }
    void AddResource(int32 resource, Player* player = NULL);
    void AddMagicPower(int32 magic, Player* player = NULL);
    void LevelUp(bool sendUpdate);
    void SetID(uint32 id) { m_ID = id; }
    uint32 GetID() { return m_ID; }
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
    uint32 m_ID; // used for saving to db.
    uint32 m_Zone;
    uint32 m_Faction;
	uint32 m_Level;
    uint32 m_Resource;
	uint32 m_MagicPower;
    uint32 m_UpgradeResource;
    uint32 m_UpgradeMagicPower;
};

#endif
