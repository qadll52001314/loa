#include "CompoundMgr.h"
#include "CompoundProcessor.h"
#include "Chat.h"
#include "ChatLink.h"
#include "LegacyMgr.h"

void CompoundProcessor::TryGemCompound(CompoundResult& result, uint32 rawStone, uint32 rawCount)
{
    const GemCompound* compound = xCompoundingMgr->FindGemCompound(rawStone, rawCount);
    if (compound)
    {
        result.success = true;
        result.result[0] = compound->gem;
        result.resultCount[0] = 1;
        result.reagent[0] = compound->rawStone;
        result.reagentCount[0] = compound->count;
    }
}

void CompoundProcessor::TryLegacySuffixCompound(Item* legacy, CompoundResult& result, CompoundReagent reagent)
{
    if (!legacy)
        return;

    uint32 enchant = legacy->GetItemRandomPropertyId();

    if (const EnchantUpgrade* upgrade = xLegacyMgr->GetEnchantUpgrade(enchant))
    {
        if (upgrade->next)
        {
            for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
            {
                if (reagent.reagent[i] == upgrade->upgradeItem)
                {
                    if (reagent.count[i] >= upgrade->upgradeItemCount)
                    {
                        result.success = true;
                        legacy->SetItemRandomProperties(upgrade->next);
                        reagent.Clear();
                        reagent.reagent[0] = upgrade->upgradeItem;
                        reagent.count[0] = upgrade->upgradeItemCount;
                        return;
                    }
                }
            }
        }
    }
}

void CompoundProcessor::TryEnchantCompound(Player* player, Item* item, CompoundResult& result, CompoundReagent reagent)
{
    const EnchantCompound* compound = xCompoundingMgr->FindEnchantCompound(player, item->GetTemplate(), reagent);
    if (compound)
    {
        result.success = true;

        for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
        {
            result.reagent[i] = compound->reagent[i];
            result.reagentCount[i] = compound->count[i];
        }

        if (compound->duration)
        {
            player->ApplyEnchantment(item, TEMP_ENCHANTMENT_SLOT, false);
            item->SetEnchantment(TEMP_ENCHANTMENT_SLOT, compound->enchant, compound->duration, 0);
            player->ApplyEnchantment(item, TEMP_ENCHANTMENT_SLOT, true);
        }
        else
        {
            player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
            item->SetEnchantment(PERM_ENCHANTMENT_SLOT, compound->enchant, 0, 0);
            player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
        }

        std::string name = ItemChatLink::FormatName(item->GetEntry());
        ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(47, name.c_str()).c_str());
        ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(43).c_str());
        for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
        {
            if (compound->reagent[i])
            {
                std::string reagentName = ItemChatLink::FormatName(compound->reagent[i]);
                ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(42, reagentName.c_str(), compound->count[i]).c_str());
            }
        }
    }
}

void CompoundProcessor::TryRecipeCompound(Player* player, CompoundResult& result, CompoundReagent reagent)
{
    const RecipeCompound* compound = xCompoundingMgr->FindRecipeCompound(player, reagent);

    if (compound)
    {
        result.success = true;

        for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
        {
            result.reagent[i] = compound->reagent[i];
            result.reagentCount[i] = compound->reagentCount[i];
        }

        for (uint8 i = 0; i != MAX_COMPOUND_ITEM; ++i)
        {
            result.result[i] = compound->item[i];
            result.result[i] = compound->itemCount[i];
        }

        // step skill if possible
        if (compound->reqSkill && player->HasSkill(compound->reqSkill))
        {
            if (player->GetSkillValue(compound->reqSkill) < compound->skillUpYellow)
                player->UpdateSkillPro(compound->reqSkill, 1000, 1);
            else if (player->GetSkillValue(compound->reqSkill) < compound->skillUpGreen)
                player->UpdateSkillPro(compound->reqSkill, 250, 1);
        }
    }
}

void CompoundProcessor::Process(Player* player, CompoundReagentItem reagent)
{
    CompoundResult result;
    result.success = false;
    result.Clear();

    Item* firstItem = reagent.reagent[0];
    CompoundReagent reagentEntries = xCompoundingMgr->StackReagents(reagent);

    if (firstItem && firstItem->GetTemplate()->Flags & ITEM_PROTO_FLAG_LEGACY)
    {
        TryLegacySuffixCompound(firstItem, result, reagentEntries);
        if (result.success)
        {
            TransportItems(player, result);
            return;
        }
    }

    // normal enchant
    if (firstItem && (firstItem->GetTemplate()->Class == ITEM_CLASS_WEAPON || firstItem->GetTemplate()->Class == ITEM_CLASS_ARMOR))
    {
        TryEnchantCompound(player, firstItem, result, reagentEntries);
        if (result.success)
            return;
    }

    // gem
    if (firstItem && xCompoundingMgr->IsRawStone(firstItem->GetTemplate()))
    {
        TryGemCompound(result, reagentEntries.reagent[0], reagentEntries.count[0]);
        if (result.success)
        {
            TransportItems(player, result);
            return;
        }
    }

    // recipe
    TryRecipeCompound(player, result, reagentEntries);
    if (result.success)
        TransportItems(player, result);
}

void CompoundProcessor::TransportItems(Player* player, CompoundResult result, bool announce /*= true*/)
{
    Item* compounder = player->GetCompoundContainer();
    if (!compounder)
        return;

    uint8 bag = compounder->GetSlot();

    // take reagents
    for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
    {
        if (result.reagent[i])
        {
            int32 reagentCount = result.reagentCount[i];
            for (uint8 j = 0; j != MAX_COMPOUND_REAGENT; ++j)
            {
                if (reagentCount > 0)
                {
                    Item* item = player->GetItemByPos(bag, j);
                    if (item && item->GetEntry() == result.reagent[i])
                    {
                        if (item->GetCount() > (uint32)reagentCount)
                        {
                            item->SetCount(item->GetCount() - reagentCount);
                            break;
                        }
                        else
                        {
                            reagentCount -= item->GetCount();
                            player->DestroyItem(bag, j, true);
                        }

                    }
                }
            }
        }
    }

    // give item
    for (uint8 i = 0; i != MAX_COMPOUND_ITEM; ++i)
    {
        if (result.result[i])
        {
            ItemPosCountVec sDest;
            // store in main bag to simplify second pass (special bags can be not equipped yet at this moment)
            InventoryResult msg = player->CanStoreNewItem(INVENTORY_SLOT_BAG_0, NULL_SLOT, sDest, result.result[i], result.resultCount[i]);
            if (msg == EQUIP_ERR_OK)
                player->StoreNewItem(sDest, result.result[i], true);
            else
            {
                SQLTransaction trans = CharacterDatabase.BeginTransaction();
                MailDraft draft = MailDraft(295);
                Item* item = Item::CreateItem(result.result[i], result.resultCount[i], player);
                if (item)
                {
                    item->SaveToDB(trans);
                    draft.AddItem(item);
                }
                draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, 43339));
                CharacterDatabase.CommitTransaction(trans);
            }
        }
    }

    // send message
    if (announce)
    {
        ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(43).c_str());
        for (uint8 i = 0; i != MAX_COMPOUND_REAGENT; ++i)
        {
            if (result.reagent[i])
            {
                std::string name = ItemChatLink::FormatName(result.reagent[i]);
                ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(42, name.c_str(), result.reagentCount).c_str());
            }
        }
        ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(41).c_str());
        for (uint8 i = 0; i != MAX_COMPOUND_ITEM; ++i)
        {
            if (result.result[i])
            {
                std::string name = ItemChatLink::FormatName(result.result[i]);
                ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(42, name.c_str(), result.resultCount).c_str());
            }
        }
    }
}
