#include "LegacyMgr.h"

void LegacyMgr::Load()
{
    LoadLearnableSpells();
    LoadLearnableTalents();

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

void LegacyMgr::LoadLearnableSpells()
{
    m_WarriorSpellMap.clear();
    m_PaladinSpellMap.clear();
    m_HunterSpellMap.clear();
    m_RogueSpellMap.clear();
    m_ShamanSpellMap.clear();
    m_MageSpellMap.clear();
    m_PriestSpellMap.clear();
    m_WarlockSpellMap.clear();
    m_DruidSpellMap.clear();
    m_DeathknightSpellMap.clear();
    //                                               0      1      2       3     4   5         6
    QueryResult result = WorldDatabase.Query("SELECT Class, Spell, Talent, Rank, Xp, ReqLevel, Next FROM player_learnable_spell");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            LearnableSpell spell;
            spell.spell = fields[1].GetUInt32();
            spell.talent = fields[2].GetBool();
            spell.rank = fields[3].GetUInt32();
            spell.xp = fields[4].GetUInt32();
            spell.reqLevel = fields[5].GetUInt32();
            spell.next = fields[6].GetUInt32();
            switch (fields[0].GetUInt32())
            {
                case SPELLFAMILY_DEATHKNIGHT:
                    m_DeathknightSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_DRUID:
                    m_DruidSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_HUNTER:
                    m_HunterSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_ROGUE:
                    m_RogueSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_MAGE:
                    m_MageSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_PRIEST:
                    m_PriestSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_SHAMAN:
                    m_ShamanSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_WARLOCK:
                    m_WarlockSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_PALADIN:
                    m_PaladinSpellMap[spell.spell] = spell;
                    break;
                case SPELLFAMILY_WARRIOR:
                    m_WarriorSpellMap[spell.spell] = spell;
                    break;
                default:
                    continue;
            }
        } while (result->NextRow());
    }
}

const LearnableSpell* LegacyMgr::GetLearnableSpellData(uint8 class_, uint32 spell) const
{
    switch (class_)
    {
        case CLASS_MAGE:
            return GetLearnableSpellData(m_MageSpellMap, spell);
        case CLASS_WARRIOR:
            return GetLearnableSpellData(m_WarriorSpellMap, spell);
        case CLASS_WARLOCK:
            return GetLearnableSpellData(m_WarlockSpellMap, spell);
        case CLASS_PRIEST:
            return GetLearnableSpellData(m_PriestSpellMap, spell);
        case CLASS_DRUID:
            return GetLearnableSpellData(m_DruidSpellMap, spell);
        case CLASS_ROGUE:
            return GetLearnableSpellData(m_RogueSpellMap, spell);
        case CLASS_HUNTER:
            return GetLearnableSpellData(m_HunterSpellMap, spell);
        case CLASS_PALADIN:
            return GetLearnableSpellData(m_PaladinSpellMap, spell);
        case CLASS_SHAMAN:
            return GetLearnableSpellData(m_ShamanSpellMap, spell);
        case CLASS_DEATH_KNIGHT:
            return GetLearnableSpellData(m_DeathknightSpellMap, spell);
        default:
            return NULL;
    }
}

const LearnableSpell* LegacyMgr::GetLearnableSpellData(const LearnableSpellMap& spellMap, uint32 spell) const
{
    LearnableSpellMap::const_iterator itr = spellMap.find(spell);
    if (itr != spellMap.end())
        return &itr->second;
    return NULL;
}

std::vector<uint32> LegacyMgr::GetLearnableSpellListForLevel(uint8 class_, uint32 level) const
{
    std::vector<uint32> list;
    switch (class_)
    {
        case CLASS_MAGE:
            list = GetLearnableSpellListForLevel(m_MageSpellMap, level);
            break;
        case CLASS_WARRIOR:
            list = GetLearnableSpellListForLevel(m_WarriorSpellMap, level);
            break;
        case CLASS_WARLOCK:
            list = GetLearnableSpellListForLevel(m_WarlockSpellMap, level);
            break;
        case CLASS_PRIEST:
            list = GetLearnableSpellListForLevel(m_PriestSpellMap, level);
            break;
        case CLASS_DRUID:
            list = GetLearnableSpellListForLevel(m_DruidSpellMap, level);
            break;
        case CLASS_ROGUE:
            list = GetLearnableSpellListForLevel(m_RogueSpellMap, level);
            break;
        case CLASS_HUNTER:
            list = GetLearnableSpellListForLevel(m_HunterSpellMap, level);
            break;
        case CLASS_PALADIN:
            list = GetLearnableSpellListForLevel(m_PaladinSpellMap, level);
            break;
        case CLASS_SHAMAN:
            list = GetLearnableSpellListForLevel(m_ShamanSpellMap, level);
            break;
        case CLASS_DEATH_KNIGHT:
            list = GetLearnableSpellListForLevel(m_DeathknightSpellMap, level);
            break;
        default:
            break;
    }

    return list;
}

std::vector<uint32> LegacyMgr::GetLearnableSpellListForLevel(const LearnableSpellMap& spellMap, uint32 level) const
{
    std::vector<uint32> list;
    LearnableSpellMap::const_iterator itr = spellMap.begin();
    while (itr != spellMap.end())
    {
        if (itr->second.reqLevel == level)
            list.push_back(itr->second.spell);
        ++itr;
    }

    return list;
}

bool LegacyMgr::IsTalentSpell(SpellFamilyNames spellFamily, uint32 spell) const
{
    if (const LearnableSpell* spellData = GetLearnableSpellData(spellFamily, spell))
        return spellData->talent;
    return false;
}

void LegacyMgr::LoadLearnableTalents()
{
    m_LearnableTalentCategoryMap.clear();
    LearnableTalentCategory category;
    category.category[0] = 161;
    category.category[1] = 163;
    category.category[2] = 164;
    m_LearnableTalentCategoryMap[CLASS_WARRIOR] = category;
    category.category[0] = 381;
    category.category[1] = 382;
    category.category[2] = 383;
    m_LearnableTalentCategoryMap[CLASS_PALADIN] = category;
    category.category[0] = 41;
    category.category[1] = 61;
    category.category[2] = 81;
    m_LearnableTalentCategoryMap[CLASS_MAGE] = category;
    category.category[0] = 181;
    category.category[1] = 182;
    category.category[2] = 183;
    m_LearnableTalentCategoryMap[CLASS_ROGUE] = category;
    category.category[0] = 201;
    category.category[1] = 202;
    category.category[2] = 203;
    m_LearnableTalentCategoryMap[CLASS_PRIEST] = category;
    category.category[0] = 261;
    category.category[1] = 262;
    category.category[2] = 263;
    m_LearnableTalentCategoryMap[CLASS_SHAMAN] = category;
    category.category[0] = 281;
    category.category[1] = 282;
    category.category[2] = 283;
    m_LearnableTalentCategoryMap[CLASS_DRUID] = category;
    category.category[0] = 301;
    category.category[1] = 302;
    category.category[2] = 303;
    m_LearnableTalentCategoryMap[CLASS_WARLOCK] = category;
    category.category[0] = 361;
    category.category[1] = 362;
    category.category[2] = 363;
    m_LearnableTalentCategoryMap[CLASS_HUNTER] = category;
    category.category[0] = 398;
    category.category[1] = 399;
    category.category[2] = 400;
    m_LearnableTalentCategoryMap[CLASS_DEATH_KNIGHT] = category;

    m_LearnableTalentMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT Spec, FirstTalent, Talent, Rank, ReqLevel FROM player_learnable_talent");

    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            LearnableTalent talent;
            talent.spec = fields[0].GetUInt32();
            talent.firstTalent = fields[1].GetUInt32();
            talent.talent = fields[2].GetUInt32();
            talent.rank = fields[3].GetUInt32();
            talent.reqLevel = fields[4].GetUInt32();
            m_LearnableTalentMap.insert(std::pair<uint32, LearnableTalent>(talent.spec, talent));
        } while (result->NextRow());
    }
}

const LearnableTalentCategory* LegacyMgr::GetLearnableTalentCategory(uint32 class_) const
{
    LearnableTalentCategoryMap::const_iterator itr = m_LearnableTalentCategoryMap.find(class_);
    if (itr != m_LearnableTalentCategoryMap.end())
        return &itr->second;
    return NULL;
}

const LearnableTalent* LegacyMgr::GetNextTalentForPlayer(Player* player) const
{
    std::vector<const LearnableTalent*> talentList;
    const LearnableTalentCategory* talentCategory = GetLearnableTalentCategory(player->getClass());
    if (!talentCategory)
        return NULL;

    int32 preferedCategory = player->GetPreferedTalentCategory();
    if (preferedCategory > 2)
        return NULL;

    if (preferedCategory <= -1) // prefered talent tier not selected.
    {
        GetAvailableTalent(talentList, player, talentCategory->category[0]);
        GetAvailableTalent(talentList, player, talentCategory->category[1]);
        GetAvailableTalent(talentList, player, talentCategory->category[2]);

        if (talentList.size() != 0)
            return talentList[urand(0, talentList.size() - 1)];
        else
            return NULL;
    }

    if (urand(0, 1))
    {
        GetAvailableTalent(talentList, player, talentCategory->category[preferedCategory]);
        if (talentList.size())
            return talentList[urand(0, talentList.size() - 1)];
    }

    GetAvailableTalent(talentList, player, talentCategory->category[0]);
    GetAvailableTalent(talentList, player, talentCategory->category[1]);
    GetAvailableTalent(talentList, player, talentCategory->category[2]);

    if (talentList.size() != 0)
        return talentList[urand(0, talentList.size() - 1)];
    else
        return NULL;
}

const LearnableTalent* LegacyMgr::GetNextTalentForPlayer(Player* player, uint32 maxLevel) const
{
    std::vector<const LearnableTalent*> talentList;
    const LearnableTalentCategory* talentCategory = GetLearnableTalentCategory(player->getClass());
    if (!talentCategory)
        return NULL;

    int32 preferedCategory = player->GetPreferedTalentCategory();
    if (preferedCategory > 2)
        return NULL;

    if (urand(0, 1))
    {
        GetAvailableTalent(talentList, player, talentCategory->category[preferedCategory], maxLevel);
        if (talentList.size())
            return talentList[urand(0, talentList.size() - 1)];
    }

    GetAvailableTalent(talentList, player, talentCategory->category[0], maxLevel);
    GetAvailableTalent(talentList, player, talentCategory->category[1], maxLevel);
    GetAvailableTalent(talentList, player, talentCategory->category[2], maxLevel);

    if (talentList.size() != 0)
        return talentList[urand(0, talentList.size() - 1)];
    else
        return NULL;
}

// @todo: only return next available rank of talents.
void LegacyMgr::GetAvailableTalent(std::vector<const LearnableTalent*>& talentList, Player* player, uint32 spec, uint32 maxLevel /* = 0*/) const
{
    LearnableTalentBounds bound = m_LearnableTalentMap.equal_range(spec);
    if (bound.first != bound.second)
    {
        for (LearnableTalentMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (player->IsTalentLearnable(&itr->second))
            {
                if (maxLevel)
                {
                    if (itr->second.reqLevel <= maxLevel)
                        talentList.push_back(&itr->second);
                }
                else
                    talentList.push_back(&itr->second);
            }
        }
    }
}

std::vector<uint32> LegacyMgr::GetSpellsForLevel(const LearnableSpellMap& spellMap, uint32 level) const
{
    std::vector<uint32> list;
    LearnableSpellMap::const_iterator itr = spellMap.begin();
    while (itr != spellMap.end())
    {
        if (itr->second.reqLevel && level > itr->second.reqLevel)
            list.push_back(itr->second.spell);
        ++itr;
    }
    return list;
}

std::vector<uint32> LegacyMgr::GetSpellsForLevel(uint8 class_, uint32 level) const
{
    std::vector<uint32> list;
    switch (class_)
    {
        case CLASS_WARRIOR:
            return GetSpellsForLevel(m_WarriorSpellMap, level);
        case CLASS_PALADIN:
            return GetSpellsForLevel(m_PaladinSpellMap, level);
        case CLASS_MAGE:
            return GetSpellsForLevel(m_MageSpellMap, level);
        case CLASS_ROGUE:
            return GetSpellsForLevel(m_RogueSpellMap, level);
        case CLASS_PRIEST:
            return GetSpellsForLevel(m_PriestSpellMap, level);
        case CLASS_SHAMAN:
            return GetSpellsForLevel(m_ShamanSpellMap, level);
        case CLASS_DRUID:
            return GetSpellsForLevel(m_DruidSpellMap, level);
        case CLASS_WARLOCK:
            return GetSpellsForLevel(m_WarlockSpellMap, level);
        case CLASS_HUNTER:
            return GetSpellsForLevel(m_HunterSpellMap, level);
        case CLASS_DEATH_KNIGHT:
            return GetSpellsForLevel(m_DeathknightSpellMap, level);
    }
    return list;
}
