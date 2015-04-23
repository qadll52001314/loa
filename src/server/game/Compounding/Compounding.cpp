#include "Compounding.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

void CompoundMgr::Load()
{
    m_RecipeCompoundMap.clear();
    QueryResult result = WorldDatabase.Query("SELECT Reagent1, Reagent2, Reagent3, Reagent4, Reagent5, Reagent6, Reagent7, Reagent8, Count1, Count2, Count3, Count4, Count5, Count6, Count7, Count8, Item1, Item2, Item3, Item4, ItemCount1, ItemCount2, ItemCount3, ItemCount4, ReqClass, ReqSkill, ReqSkillValue, SkillUpYellow, SkillUpGreen, ReqSpellFocus, ReqTotem1, ReqTotem2, ReqTool1, ReqTool2, Cooldown, CDCategory FROM compound_recipe WHERE Enabled <> 0");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            RecipeCompound compound;
            uint8 x = 0;
            for (uint8 i = x; i != MAX_COMPOUND_REAGENT; ++i)
                compound.reagent[i] = fields[i].GetUInt32();
            x += MAX_COMPOUND_REAGENT;
            for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
                compound.reagentCount[i] = fields[i + x].GetUInt32();
            x += MAX_COMPOUND_REAGENT;
            for (uint8 i = 0; i != MAX_COMPOUND_ITEM; ++i)
                compound.item[i] = fields[i + x].GetUInt32();
            x += MAX_COMPOUND_ITEM;
            for (uint8 i = 0; i != MAX_COMPOUND_ITEM; ++i)
                compound.itemCount[i] = fields[i + x].GetUInt32();
            x += MAX_COMPOUND_ITEM;
            compound.reqClass = fields[x].GetUInt32();
            compound.reqSkill = fields[x + 1].GetUInt32();
            compound.reqSkillValue = fields[x + 2].GetUInt32();
            compound.skillUpYellow = fields[x + 3].GetUInt32();
            compound.skillUpGreen = fields[x + 4].GetUInt32();
            compound.reqSpellFocus = fields[x + 5].GetUInt32();
            compound.reqTotem1 = fields[x + 6].GetUInt32();
            compound.reqTotem2 = fields[x + 7].GetUInt32();
            compound.ReqTool1 = fields[x + 8].GetUInt32();
            compound.ReqTool2 = fields[x + 9].GetUInt32();
            compound.cooldown = fields[x + 10].GetUInt32();
            compound.cdCategory = fields[x + 11].GetUInt32();
            m_RecipeCompoundMap.insert(std::pair<uint32, RecipeCompound>(compound.reagent[0], compound));
        } while (result->NextRow());
    }

    m_GemCompoundMap.clear();

    result = WorldDatabase.Query("SELECT Raw, Count, Gem, Chance FROM compound_gem");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            GemCompound compound;
            compound.rawStone = fields[0].GetUInt32();
            compound.count = fields[1].GetUInt32();
            compound.gem = fields[2].GetUInt32();
            compound.chance = fields[3].GetUInt32();
            m_GemCompoundMap.insert(std::pair<uint32, GemCompound>(compound.rawStone, compound));
        } while (result->NextRow());
    }

    m_EnchantCompoundMap.clear();
    result = WorldDatabase.Query("SELECT Enchant, Duration, Class, Subclass, InventoryType, Reagent1, Reagent2, Reagent3, Reagent4, Reagent5, Reagent6, Reagent7, Reagent8, Count1, Count2, Count3, Count4, Count5, Count6, Count7, Count8, ReqSkill, ReqSkillValue FROM compound_enchant");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            EnchantCompound compound;
            compound.enchant = fields[0].GetUInt32();
            compound.duration = fields[1].GetUInt32();
            compound._class = fields[2].GetUInt32();
            compound.subclass = fields[3].GetUInt32();
            compound.inventoryType = fields[4].GetUInt32();
            compound.reagent[0] = fields[5].GetUInt32();
            compound.reagent[1] = fields[6].GetUInt32();
            compound.reagent[2] = fields[7].GetUInt32();
            compound.reagent[3] = fields[8].GetUInt32();
            compound.reagent[4] = fields[9].GetUInt32();
            compound.reagent[5] = fields[10].GetUInt32();
            compound.reagent[6] = fields[11].GetUInt32();
            compound.reagent[7] = fields[12].GetUInt32();
            compound.count[0] = fields[13].GetUInt32();
            compound.count[1] = fields[14].GetUInt32();
            compound.count[2] = fields[15].GetUInt32();
            compound.count[3] = fields[16].GetUInt32();
            compound.count[4] = fields[17].GetUInt32();
            compound.count[5] = fields[18].GetUInt32();
            compound.count[6] = fields[19].GetUInt32();
            compound.count[7] = fields[20].GetUInt32();
            compound.reqSkill = fields[21].GetUInt32();
            compound.reqSkillValue = fields[22].GetUInt32();
            m_EnchantCompoundMap.insert(std::pair<uint32, EnchantCompound>(compound.reagent[0], compound));
        } while (result->NextRow());
    }

    m_SocketCompoundMap.clear();
    result = WorldDatabase.Query("SELECT Stone, Count, MaxItemLevel FROM compound_socket");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            SocketCompound compound;
            compound.stone = fields[0].GetUInt32();
            compound.count = fields[1].GetUInt32();
            compound.maxItemLevel = fields[2].GetUInt32();
            m_SocketCompoundMap.insert(std::pair<uint32, SocketCompound>(fields[0].GetUInt32(), compound));
        } while (result->NextRow());
    }
}

RecipeCompoundBounds CompoundMgr::GetRecipeCompoundBounds(uint32 reagent) const
{
    return m_RecipeCompoundMap.equal_range(reagent);
}

uint32 CompoundMgr::CanCompoundRecipe(Reagent reagent, const RecipeCompound* compound, const Player* player) const
{
    uint32 point = 0;

    //if (compound->reqSkill)
    //{
    //    if (player->GetSkillValue(compound->reqSkill) < compound->reqSkillValue)
    //        return 0;
    //}

    if (compound->reqSpellFocus)
    {
        if (!IsNearSpellFocus(player, compound->reqSpellFocus))
            return 0;
        else
            point += 10000;
    }

    if (compound->reqTotem1)
    {
        if (!player->HasItemTotemCategory(compound->reqTotem1))
            return 0;
        else 
            point += 10000;
    }

    if (compound->reqTotem2)
    {
        if (!player->HasItemTotemCategory(compound->reqTotem2))
            return 0;
        else
            point += 10000;
    }

    if (compound->ReqTool1)
    {
        if (!player->HasItemCount(compound->ReqTool1))
            return 0;
        else
            point += 10000;
    }

    if (compound->ReqTool2)
    {
        if (!player->HasItemCount(compound->ReqTool2))
            return 0;
        else
            point += 10000;
    }

    uint32 next = 0;

    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (compound->reagent[i])
        {
            bool hasReagent = false;
            for (uint8 j = 0; j != MAX_COMPOUND_REAGENT; ++j)
            {
                if (reagent.reagent[j] == compound->reagent[i])
                {
                    if (reagent.count[j] < compound->reagentCount[i])
                        return 0;
                    hasReagent = true;
                    point += compound->reagentCount[i] + next * 255;
                    ++next;
                }
            }
            if (!hasReagent)
                return 0;
        }
    }

    return point;
}

void CompoundMgr::StackReagents(Reagent& reagent) const
{
    for (uint8 i = MAX_COMPOUND_REAGENT - 1; i != 0; --i)
    {
        if (reagent.reagent[i])
        {
            for (uint8 j = 0; j < i; ++j)
            {
                if (!reagent.reagent[j])
                {
                    reagent.reagent[j] = reagent.reagent[i];
                    reagent.count[j] = reagent.count[i];
                    reagent.reagent[i] = 0;
                    reagent.count[i] = 0;
                    break;
                }
            }
        }
    }

    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (reagent.reagent[i])
        {
            for (uint8 j = i + 1; j < MAX_COMPOUND_REAGENT; ++j)
            {
                if (reagent.reagent[i] == reagent.reagent[j])
                {
                    reagent.count[i] += reagent.count[j];
                    reagent.reagent[j] = 0;
                    reagent.count[j] = 0;
                }
            }
        }
    }
}

bool CompoundMgr::IsNearSpellFocus(const Player* player, uint32 spellFocus) const
{
    GameObject* focus = NULL;
    Trinity::GameObjectFocusCheck check(player, spellFocus);
    Trinity::GameObjectSearcher<Trinity::GameObjectFocusCheck> searcher(player, focus, check);

    float x, y;
    x = player->GetPositionX();
    y = player->GetPositionY();

    CellCoord p(Trinity::ComputeCellCoord(x, y));
    Cell cell(p);
    cell.SetNoCreate();

    Map& map = *(player->GetMap());

    TypeContainerVisitor<Trinity::GameObjectSearcher<Trinity::GameObjectFocusCheck>, GridTypeMapContainer >  grid_object_notifier(searcher);
    cell.Visit(p, grid_object_notifier, map, player->GetVisibilityRange(), x, y);

    if (focus)
        return true;
    return false;
}

const RecipeCompound* CompoundMgr::TryRecipeCompound(const Player* player, Reagent reagent) const
{
    const RecipeCompound* ptr = NULL;
    uint32 hitPoint = 0;

    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (reagent.reagent[i])
        {
            RecipeCompoundBounds bound = GetRecipeCompoundBounds(reagent.reagent[i]);
            if (bound.first == bound.second)
                continue;

            for (RecipeCompoundMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
            {
                uint32 point = CanCompoundRecipe(reagent, &itr->second, player);
                if (point && point > hitPoint)
                {
                    hitPoint = point;
                    ptr = &itr->second;
                }
            }
        }
    }

    return ptr;
}

const GemCompound* CompoundMgr::TryGemCompound(const Player* player, Reagent reagent) const
{
    GemCompoundBounds bound = GetGemCompoundBounds(reagent.reagent[0]);
    if (bound.first == bound.second)
        return NULL;

    uint32 totalChance = 0;
    uint32 totalFormula = 0;

    for (GemCompoundMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (reagent.count[0] >= (*itr).second.count)
        {
            totalChance += (*itr).second.chance;
            totalFormula++;
        }
    }

    if (!totalChance) // equal chance case
    {
        totalChance = totalFormula * 100;
        uint32 roll = urand(0, totalChance);
        uint32 tmp = 0;
        for (GemCompoundMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (reagent.count[0] >= (*itr).second.count)
            {
                tmp += 100;
                if (tmp > roll)
                    return &(*itr).second;
            }
        }
    }
    else
    {
        uint32 roll = urand(0, totalChance);
        uint32 tmp = 0;
        for (GemCompoundMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (reagent.count[0] >= (*itr).second.count)
            {
                tmp += (*itr).second.chance;
                if (tmp > roll)
                    return &(*itr).second;
            }
        }
    }

    return NULL;
}

GemCompoundBounds CompoundMgr::GetGemCompoundBounds(uint32 raw) const
{
    return m_GemCompoundMap.equal_range(raw);
}

bool CompoundMgr::IsRawStone(const ItemTemplate* item) const
{
    GemCompoundMap::const_iterator itr = m_GemCompoundMap.find(item->ItemId);
    return itr != m_GemCompoundMap.end();
}

void CompoundMgr::PickGemReagent(const GemCompound* compound, Reagent& reagent) const
{
    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        reagent.reagent[i] = 0;
        reagent.count[i] = 0;
    }

    reagent.reagent[0] = compound->rawStone;
    reagent.count[0] = compound->count;
}

const EnchantCompound* CompoundMgr::TryEnchantCompound(const Player* player, const ItemTemplate* item, Reagent reagent) const
{
    const EnchantCompound* ptr = 0;
    uint32 hitPoint = 0;

    for (uint8 i = 1; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (reagent.reagent[i])
        {
            EnchantCompoundBounds bound = GetEnchantCompoundBounds(reagent.reagent[i]);
            if (bound.first != bound.second)
            {
                for (EnchantCompoundMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
                {
                    uint32 point = CanCompoundEnchant(item, reagent, &(*itr).second, player);
                    if (point > hitPoint)
                    {
                        hitPoint = point;
                        ptr = &(*itr).second;
                    }
                }
            }
        }
    }

    return ptr;
}

EnchantCompoundBounds CompoundMgr::GetEnchantCompoundBounds(uint32 enchant) const
{
    return m_EnchantCompoundMap.equal_range(enchant);
}

uint32 CompoundMgr::CanCompoundEnchant(const ItemTemplate* item, Reagent reagent, const EnchantCompound* compound, const Player* player) const
{
    uint32 point = 0;

    //if (compound->reqSkill)
    //{
    //    if (player->GetSkillValue(compound->reqSkill) < compound->reqSkillValue)
    //        return 0;
    //}
    //else
    //    point += 1000;

    if (compound->_class)
    {
        if (item->Class != compound->_class || !(compound->subclass & (1 << item->SubClass)))
            return 0;
        else
            point += 1000;
    }

    if (compound->inventoryType)
    {
        if (!(compound->inventoryType & (1 << item->InventoryType)))
            return 0;
        else
            point += 1000;
    }

    uint32 next = 0;

    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (compound->reagent[i])
        {
            bool hasReagent = false;
            for (uint8 j = 0; j != MAX_COMPOUND_REAGENT; ++j)
            {
                if (reagent.reagent[j] == compound->reagent[i])
                {
                    if (reagent.count[j] < compound->count[i])
                        return 0;
                    hasReagent = true;
                    point += compound->count[i] + next * 255;
                    ++next;
                }
            }
            if (!hasReagent)
                return 0;
        }
    }

    return point;
}

bool CompoundMgr::IsSocketStone(const ItemTemplate* item) const
{
    SocketCompoundMap::const_iterator itr = m_SocketCompoundMap.find(item->ItemId);
    return itr != m_SocketCompoundMap.end();
}

const SocketCompound* CompoundMgr::TrySocketCompound(const Player* player, const ItemTemplate* item, Reagent reagent) const
{
    const SocketCompound* ptr = NULL;

    SocketCompoundBounds bound = GetSocketCompoundBounds(reagent.reagent[0]);

    uint32 hitPoint = 0;

    if (bound.first != bound.second)
    {
        for (SocketCompoundMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
        {
            uint32 point = CanCompoundSocket(item, reagent, &(*itr).second);
            if (point > hitPoint)
            {
                hitPoint = point;
                ptr = &(*itr).second;
            }
        }
    }

    return ptr;
}

uint32 CompoundMgr::CanCompoundSocket(const ItemTemplate* item, Reagent reagent, const SocketCompound* compound) const
{
    if (item->ItemLevel > compound->maxItemLevel)
        return 0;

    if (reagent.reagent[0] == compound->stone && reagent.count[0] >= compound->count)
        return compound->count;

    return 0;
}

SocketCompoundBounds CompoundMgr::GetSocketCompoundBounds(uint32 stone) const
{
    return m_SocketCompoundMap.equal_range(stone);
}

void CompoundMgr::PickSocketReagent(const SocketCompound* compound, Reagent& reagent) const
{
    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        reagent.reagent[i] = 0;
        reagent.count[i] = 0;
    }

    reagent.reagent[0] = compound->stone;
    reagent.count[0] = compound->count;
}

uint32 CompoundMgr::CalculateRecycleAmount(Reagent& reagent) const
{
    uint32 amount = 0;
    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (const ItemTemplate* item = sObjectMgr->GetItemTemplate(reagent.reagent[i]))
        {
            float point = 0;
            if (item->Class == ITEM_CLASS_WEAPON || item->Class == ITEM_CLASS_ARMOR)
                point = item->ItemLevel * 2;
            else if (item->Class == ITEM_CLASS_CONSUMABLE)
                point = item->ItemLevel / 4;
            else if (item->Class == ITEM_CLASS_GEM)
                point = item->ItemLevel / 2;
            else if (item->Class == ITEM_CLASS_REAGENT)
                point = item->ItemLevel / 3;
            else if (item->Class == ITEM_CLASS_TRADE_GOODS)
                point = item->ItemLevel / 2;
            else if (item->Class == ITEM_CLASS_RECIPE)
                point = item->ItemLevel / 4;
            else
            {
                reagent.reagent[i] = 0;
                reagent.count[i] = 0;
                continue;
            }
            if (point < 1) point = 1;
            switch (item->Quality)
            {
                case ITEM_QUALITY_NORMAL:
                    point *= 1.2f;
                    break;
                case ITEM_QUALITY_UNCOMMON:
                    point *= 1.6f;
                    break;
                case ITEM_QUALITY_RARE:
                    point *= 2.2f;
                    break;
                case ITEM_QUALITY_EPIC:
                    point *= 3.0f;
                    break;
                case ITEM_QUALITY_LEGENDARY:
                    point *= 4.0f;
                    break;
            }
            point *= reagent.count[i];
            point *= frand(0.8f, 1.2f);
            amount += point;
        }
    }
    return amount;
}
