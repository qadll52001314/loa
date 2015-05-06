#include "LegacyMgr.h"

void LegacyMgr::Load()
{
    m_ItemLootCooldownMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT Item, Cooldown FROM item_loot_cooldown");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_ItemLootCooldownMap[fields[0].GetUInt32()] = fields[1].GetUInt32();
        } while (result->NextRow());
    }

    m_ItemUpgradeMap.clear();
    result = WorldDatabase.Query("SELECT Item, Xp, Next FROM item_upgrade_data");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            ItemUpgrade upgrade;
            upgrade.item = fields[0].GetUInt32();
            upgrade.xp = fields[1].GetUInt32();
            upgrade.next = fields[2].GetUInt32();
            m_ItemUpgradeMap[upgrade.item] = upgrade;
        } while (result->NextRow());
    }

    m_ItemLegacyMap.clear();
    result = WorldDatabase.Query("SELECT Prototype, Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8, Rank9, Rank10, Rank11, Rank12, Rank13, Rank14, Rank15 FROM item_legacy_data");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            ItemLegacy legacy;
            legacy.prototype = fields[0].GetUInt32();
            for (uint8 i = 0; i != MAX_LEGACY_RANK; ++i)
                legacy.legacy[i] = fields[i + 1].GetUInt32();
            m_ItemLegacyMap[legacy.prototype] = legacy;
        } while (result->NextRow());
    }

    m_ItemEnchantMap.clear();
    result = WorldDatabase.Query("SELECT Enchant, Rank, Next, UpgradeItem, UpgradeItemCount FROM item_enchant_upgrade_data");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            EnchantUpgrade enchant;
            enchant.enchant = fields[0].GetUInt32();
            enchant.rank = fields[1].GetUInt32();
            enchant.next = fields[2].GetUInt32();
            enchant.upgradeItem = fields[3].GetUInt32();
            enchant.upgradeItemCount = fields[4].GetUInt32();
            m_ItemEnchantMap[enchant.enchant] = enchant;
        } while (result->NextRow());
    }

    m_EnchantDataMap.clear();
    result = WorldDatabase.Query("SELECT FirstRank, Rank, ID FROM item_enchant_data");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            EnchantData data;
            data.enchant = fields[0].GetUInt32();
            data.rank = fields[1].GetUInt32();
            data.id = fields[2].GetUInt32();
            m_EnchantDataMap.insert(std::pair<uint32, EnchantData>(data.enchant, data));
        } while (result->NextRow());
    }

    m_EnchantChanceMap.clear();
    result = WorldDatabase.Query("SELECT Enchant, Chance FROM item_enchant_chance");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            m_EnchantChanceMap[fields[0].GetUInt32()] = fields[1].GetFloat();
            m_EnchantChanceSum += fields[1].GetFloat();
        } while (result->NextRow());
    }
}

uint32 LegacyMgr::GetLootCooldown(uint32 item) const
{
    ItemLootCooldownMap::const_iterator itr = m_ItemLootCooldownMap.find(item);
    if (itr == m_ItemLootCooldownMap.end())
        return 0;
    return itr->second;
}

uint32 LegacyMgr::GetUpgradeOf(uint32 item, uint32 rank) const
{
    if (rank < 1 || rank >= MAX_LEGACY_RANK)
        return 0;
    ItemLegacyMap::const_iterator itr = m_ItemLegacyMap.find(item);
    if (itr != m_ItemLegacyMap.end())
        return itr->second.legacy[rank - 1];
    return 0;
}

uint32 LegacyMgr::GetLegacyOf(uint32 item) const
{
    ItemLegacyMap::const_iterator itr = m_ItemLegacyMap.find(item);
    if (itr != m_ItemLegacyMap.end())
        return itr->second.legacy[0];
    return 0;
}

const ItemUpgrade* LegacyMgr::GetItemUpgrade(uint32 item) const
{
    ItemUpgradeMap::const_iterator itr = m_ItemUpgradeMap.find(item);
    if (itr == m_ItemUpgradeMap.end())
        return NULL;
    return &itr->second;
}

const EnchantUpgrade* LegacyMgr::GetEnchantUpgrade(uint32 enchant) const
{
    EnchantUpgradeMap::const_iterator itr = m_ItemEnchantMap.find(enchant);
    if (itr == m_ItemEnchantMap.end())
        return NULL;
    return &itr->second;
}

int32 LegacyMgr::RollLegacyEnchant(const ItemTemplate* item/* = NULL*/) const
{
    float chance = frand(0, m_EnchantChanceSum);
    float tmp = 0;
    for (EnchantChanceMap::const_iterator itr = m_EnchantChanceMap.begin(); itr != m_EnchantChanceMap.end(); ++itr)
    {
        tmp += itr->second;
        if (tmp >= chance)
            return itr->first;
    }
    return 0;
}

int32 LegacyMgr::RollLegacyEnchantOfRank(uint32 rank) const
{
    int32 enchant = RollLegacyEnchant();
    if (enchant)
    {
        EnchantDataBounds bound = m_EnchantDataMap.equal_range(enchant);
        if (bound.first == bound.second)
            return 0;

        for (EnchantDataMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.rank == rank)
                return itr->second.id;
        }
    }

    return 0;
}
