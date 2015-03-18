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
    CapitalCity() { m_Zone = 0; m_Team = 0; m_Rank = 1; m_Resource = 0; m_MagicPower = 0; m_UpgradeResource = 0; m_UpgradeMagicPower = 0; m_Upgrading = false; }
    ~CapitalCity() { }

    void Update();
    void SetResource(uint32 resource) { m_Resource = resource; }
    void SetRank(uint32 rank) { m_Rank = rank; }
    uint32 GetRank() { return m_Rank; }
    uint32 GetResource() { return m_Resource; }
	void SetMagicPower(uint32 magic) { m_MagicPower = magic; }
	uint32 GetMagicPower() { return m_MagicPower; }
    void SetUpgradeResource(uint32 resource) { m_UpgradeResource = resource; }
    uint32 GetUpgradeResource() { return m_UpgradeResource; }
    void SetUpgradeMagicPower(uint32 magic) { m_UpgradeMagicPower = magic; }
    uint32 GetUpgradeMagicPower() { return m_UpgradeMagicPower; }
    void SetTeam(uint32 team) { m_Team = team; }
    uint32 GetTeam() { return m_Team; }
    void SetZone(uint32 zone) { m_Zone = zone; }
    uint32 GetZone() { return m_Zone; }
    void AddResource(int32 resource, Player* player = NULL);
    void AddMagicPower(int32 magic, Player* player = NULL);
    void RankUp(bool sendUpdate);
    void SetID(uint32 id) { m_ID = id; }
    uint32 GetID() { return m_ID; }
    std::string GetName();
    void UpgradeStart(Player* commander = NULL);
    void UpgradeStop(Player* commander = NULL);

    void Save();

    virtual void GatherResource() { }

    void HandlePlayerEnter(Player*);
    void HandlePlayerLeave(Player*);

    void SendStateUpdate();
    void ClearState(uint32 index);
    void SendAllState(Player*);
    void ClearAllState(Player*);

    void SetUpgradeState(bool upgrading) { m_Upgrading = upgrading; }

    void LearnRankUpSpells(uint32 rank);
private:
    uint32 m_ID; // used for saving to db.
    uint32 m_Zone;
    uint32 m_Team;
	uint32 m_Rank;
    uint32 m_Resource;
	uint32 m_MagicPower;
    uint32 m_UpgradeResource;
    uint32 m_UpgradeMagicPower;
    bool m_Upgrading;
};

#endif
