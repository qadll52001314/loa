#ifndef _LEGACY_H_
#define _LEGACY_H_

#include "Player.h"

#define MAX_LEGACY_RANK 15

struct ItemUpgrade
{
    uint32 item;
    uint32 xp;
    uint32 next;
};

struct EnchantUpgrade
{
    uint32 enchant;
    uint32 rank;
    uint32 upgradeItem;
    uint32 upgradeItemCount;
    uint32 next;
};

struct EnchantData
{
    uint32 enchant;
    uint32 rank;
    uint32 id;
};

struct ItemLegacy
{
    uint32 prototype;
    uint32 legacy[MAX_LEGACY_RANK];
};

struct LearnableSpell
{
    uint32 spell;
    uint32 rank;
    uint32 xp;
    uint32 reqLevel;
    uint32 next;
    bool talent;
};

struct LearnableTalent
{
    uint32 spec;
    uint32 firstTalent;
    uint32 talent;
    uint32 rank;
    uint32 reqLevel;
};

struct LearnableTalentCategory
{
    uint32 category[3];
};

typedef std::map<uint32, ItemUpgrade> ItemUpgradeMap;
typedef std::map<uint32, EnchantUpgrade> EnchantUpgradeMap;
typedef std::map<uint32, ItemLegacy> ItemLegacyMap;
typedef std::map<uint32, float> EnchantChanceMap;
typedef std::multimap<uint32, EnchantData> EnchantDataMap;
typedef std::pair<EnchantDataMap::const_iterator, EnchantDataMap::const_iterator> EnchantDataBounds;
typedef std::map<uint32, uint32> ItemLootCooldownMap;

typedef std::map<uint32, LearnableSpell> LearnableSpellMap;
typedef std::multimap<uint32, LearnableTalent> LearnableTalentMap;
typedef std::map<uint32, LearnableTalentCategory> LearnableTalentCategoryMap;

typedef std::pair<LearnableTalentMap::const_iterator, LearnableTalentMap::const_iterator> LearnableTalentBounds;

class LegacyMgr
{
public:
    static LegacyMgr* instance()
    {
        static LegacyMgr instance;
        return &instance;
    }

    void Load();

    uint32 GetLootCooldown(uint32 item) const;
    uint32 GetUpgradeOf(uint32 item, uint32 rank) const;
    uint32 GetLegacyOf(uint32 item) const;
    const ItemUpgrade* GetItemUpgrade(uint32 item) const;
    const EnchantUpgrade* GetEnchantUpgrade(uint32 enchant) const;

    int32 RollLegacyEnchant(const ItemTemplate* item = NULL) const;
    int32 RollLegacyEnchantOfRank(uint32 rank) const;

    void LoadLearnableSpells();

    const LearnableSpell* GetLearnableSpellData(uint8 class_, uint32 spell) const;
    const LearnableSpell* GetLearnableSpellData(const LearnableSpellMap& spellMap, uint32 spell) const;
    std::vector<uint32> GetLearnableSpellListForLevel(uint8 class_, uint32 level) const;
    std::vector<uint32> GetLearnableSpellListForLevel(const LearnableSpellMap& spellMap, uint32 level) const;
    std::vector<uint32> GetSpellsForLevel(const LearnableSpellMap& spellMap, uint32 level) const;
    std::vector<uint32> GetSpellsForLevel(uint8 class_, uint32 level) const;

    bool IsTalentSpell(SpellFamilyNames spellFamily, uint32 spell) const;
    void LoadLearnableTalents();
    const LearnableTalent* GetNextTalentForPlayer(Player* player) const;
    const LearnableTalent* GetNextTalentForPlayer(Player* player, uint32 maxLevel) const;
    void GetAvailableTalent(std::vector<const LearnableTalent*>& talentList, Player* player, uint32 spec, uint32 maxLevel = 0) const;
    const LearnableTalentCategory* GetLearnableTalentCategory(uint32 class_) const;

private:
    ItemLootCooldownMap m_ItemLootCooldownMap;
    ItemUpgradeMap m_ItemUpgradeMap;
    EnchantUpgradeMap m_ItemEnchantMap;
    ItemLegacyMap m_ItemLegacyMap;
    EnchantChanceMap m_EnchantChanceMap;
    float m_EnchantChanceSum;
    EnchantDataMap m_EnchantDataMap;

    LearnableSpellMap m_WarriorSpellMap;
    LearnableSpellMap m_PaladinSpellMap;
    LearnableSpellMap m_HunterSpellMap;
    LearnableSpellMap m_RogueSpellMap;
    LearnableSpellMap m_ShamanSpellMap;
    LearnableSpellMap m_MageSpellMap;
    LearnableSpellMap m_PriestSpellMap;
    LearnableSpellMap m_WarlockSpellMap;
    LearnableSpellMap m_DruidSpellMap;
    LearnableSpellMap m_DeathknightSpellMap;

    LearnableTalentMap m_LearnableTalentMap;
    LearnableTalentCategoryMap m_LearnableTalentCategoryMap;
};

#define xLegacyMgr LegacyMgr::instance()

#endif
