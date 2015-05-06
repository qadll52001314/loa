#ifndef _COMPOUNDPROCESSOR_H_
#define _COMPOUNDPROCESSOR_H_

#include "Player.h"

class CompoundProcessor
{
public:
    static void Process(Player* player, CompoundReagentItem reagent);

private:
    static void TryLegacySuffixCompound(Item* legacy, CompoundResult& result, CompoundReagent reagent);
    static void TryRecipeCompound(Player* player, CompoundResult& result, CompoundReagent reagent);
    static void TryGemCompound(CompoundResult& result, uint32 rawStone, uint32 rawCount);
    static void TryEnchantCompound(Player* player, Item* item, CompoundResult& result, CompoundReagent reagent);
    static void TrySocketCompound(Item* item, CompoundResult& result, CompoundReagent reagent);
    static void TransportItems(Player* player, CompoundResult result, bool announce = true);
};

#endif