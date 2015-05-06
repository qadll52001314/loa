#ifndef _LEGACY_H_
#define _LEGACY_H_

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

typedef std::map<uint32, ItemUpgrade> ItemUpgradeMap;
typedef std::map<uint32, EnchantUpgrade> EnchantUpgradeMap;
typedef std::map<uint32, ItemLegacy> ItemLegacyMap;
typedef std::map<uint32, float> EnchantChanceMap;
typedef std::multimap<uint32, EnchantData> EnchantDataMap;
typedef std::pair<EnchantDataMap::const_iterator, EnchantDataMap::const_iterator> EnchantDataBounds;
typedef std::map<uint32, uint32> ItemLootCooldownMap;

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

private:
    ItemLootCooldownMap m_ItemLootCooldownMap;
    ItemUpgradeMap m_ItemUpgradeMap;
    EnchantUpgradeMap m_ItemEnchantMap;
    ItemLegacyMap m_ItemLegacyMap;
    EnchantChanceMap m_EnchantChanceMap;
    float m_EnchantChanceSum;
    EnchantDataMap m_EnchantDataMap;
};

#define xLegacyMgr LegacyMgr::instance()

#endif
