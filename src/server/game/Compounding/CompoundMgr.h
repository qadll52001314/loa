#ifndef _COMPOUNDMGR_H_
#define _COMPOUNDMGR_H_

#define MAX_COMPOUND_REAGENT 8
#define MAX_COMPOUND_ITEM 4

struct RecipeCompound
{
    uint32 reagent[MAX_COMPOUND_REAGENT];
    uint32 reagentCount[MAX_COMPOUND_REAGENT];
    uint32 item[MAX_COMPOUND_ITEM];
    uint32 itemCount[MAX_COMPOUND_ITEM];
    uint32 reqClass;
    uint32 reqSkill;
    uint32 reqSkillValue;
    uint32 skillUpYellow;
    uint32 skillUpGreen;
    uint32 reqSpellFocus;
    uint32 reqTotem1;
    uint32 reqTotem2;
    uint32 ReqTool1;
    uint32 ReqTool2;
    uint32 cooldown;
    uint32 cdCategory;
};

struct GemCompound
{
    uint32 rawStone;
    uint32 count;
    uint32 gem;
    float chance;
};

struct EnchantCompound
{
    uint32 enchant;
    uint32 duration;
    uint32 _class;
    uint32 subclass;
    uint32 inventoryType;
    uint32 reagent[MAX_COMPOUND_REAGENT];
    uint32 count[MAX_COMPOUND_REAGENT];
    uint32 reqSkill;
    uint32 reqSkillValue;
};

struct SocketCompound
{
    uint32 stone;
    uint32 count;
    uint32 maxItemLevel;
};

struct CompoundReagentItem
{
    Item* reagent[MAX_COMPOUND_REAGENT];
};

struct CompoundReagent
{
    uint32 reagent[MAX_COMPOUND_REAGENT];
    uint32 count[MAX_COMPOUND_REAGENT];

    void Clear()
    {
        for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
        {
            reagent[i] = 0;
            count[i] = 0;
        }
    }
};

typedef std::multimap<uint32, RecipeCompound> RecipeCompoundMap;
typedef std::pair<RecipeCompoundMap::const_iterator, RecipeCompoundMap::const_iterator> RecipeCompoundBounds;
typedef std::multimap<uint32, GemCompound> GemCompoundMap;
typedef std::pair<GemCompoundMap::const_iterator, GemCompoundMap::const_iterator> GemCompoundBounds;
typedef std::multimap<uint32, EnchantCompound> EnchantCompoundMap;
typedef std::pair<EnchantCompoundMap::const_iterator, EnchantCompoundMap::const_iterator> EnchantCompoundBounds;
typedef std::multimap<uint32, SocketCompound> SocketCompoundMap;
typedef std::pair<SocketCompoundMap::const_iterator, SocketCompoundMap::const_iterator> SocketCompoundBounds;

struct CompoundResult
{
    bool success;
    uint32 reagent[MAX_COMPOUND_REAGENT];
    uint32 reagentCount[MAX_COMPOUND_REAGENT];
    uint32 result[MAX_COMPOUND_ITEM];
    uint32 resultCount[MAX_COMPOUND_ITEM];
    void Clear()
    {
        for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
        {
            reagent[i] = 0;
            reagentCount[i] = 0;
        }

        for (uint8 i = 0; i != MAX_COMPOUND_ITEM; ++i)
        {
            result[i] = 0;
            resultCount[i] = 0;
        }
    }
};

class CompoundMgr
{
public:
    static CompoundMgr* instance()
    {
        static CompoundMgr instance;
        return &instance;
    }

    CompoundMgr() { }
    ~CompoundMgr() { }

    void Load();

    void ProcessCompound(Player* player, CompoundReagentItem reagent) const;
    CompoundReagent StackReagents(const CompoundReagentItem& reagent) const;

    RecipeCompoundBounds GetRecipeCompoundBounds(uint32 reagent) const;
    GemCompoundBounds GetGemCompoundBounds(uint32 raw) const;
    EnchantCompoundBounds GetEnchantCompoundBounds(uint32 enchant) const;
    SocketCompoundBounds GetSocketCompoundBounds(uint32 stone) const;

    const RecipeCompound* FindRecipeCompound(const Player* player, CompoundReagent reagent) const;
    const GemCompound* FindGemCompound(uint32 rawStone, uint32 rawCount) const;
    const EnchantCompound* FindEnchantCompound(Player* player, const ItemTemplate* item, CompoundReagent reagent) const;
    const SocketCompound* FindSocketCompound(const Player* player, const ItemTemplate* item, CompoundReagent reagent) const;
    void FindLegacyCompound(CompoundResult& result, CompoundReagent& reagent) const;

    uint32 CanCompoundRecipe(CompoundReagent reagent, const RecipeCompound* compound, const Player* player) const;
    uint32 CanCompoundEnchant(const ItemTemplate* item, CompoundReagent reagent, const EnchantCompound* compound, const Player* player) const;
    uint32 CanCompoundSocket(const ItemTemplate* item, CompoundReagent reagent, const SocketCompound* compound) const;

    bool IsNearSpellFocus(const Player* player, uint32 spellFocus) const;
    bool IsRawStone(const ItemTemplate* item) const;
    bool IsSocketStone(const ItemTemplate* item) const;

    uint32 CalculateRecycleAmount(CompoundReagent& reagent) const;

private:
    RecipeCompoundMap m_RecipeCompoundMap;
    GemCompoundMap m_GemCompoundMap;
    EnchantCompoundMap m_EnchantCompoundMap;
    SocketCompoundMap m_SocketCompoundMap;
};

#define xCompoundingMgr CompoundMgr::instance()
#endif // !_H_COMPOUNDING_


