#include "MemoryMgr.h"
#include "WorldSession.h"
#include "ChatLink.h"
#include "Chat.h"

void MemoryMgr::LoadDefine()
{
    m_CollectableMemoryMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT ID, ReqItem, ReqItemCount, RewardItem, RewardItemCount, RewardSpell, Mail, Sender, Text FROM memory");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CollectableMemory memory;
            memory.id = fields[0].GetUInt32();
            memory.reqItem = fields[1].GetUInt32();
            memory.reqItemCount = fields[2].GetUInt32();
            memory.rewardItem = fields[3].GetUInt32();
            memory.rewardItemCount = fields[4].GetUInt32();
            memory.rewardSpell = fields[5].GetUInt32();
            memory.mail = fields[6].GetUInt32();
            memory.sender = fields[7].GetUInt32();
            memory.text = fields[8].GetUInt32();
            m_CollectableMemoryMap.insert(std::pair<uint32, CollectableMemory>(memory.id, memory));
        } while (result->NextRow());
    }

    m_AccountCollectedMemoryMap.clear();
    result = LoginDatabase.Query("SELECT Account, Memory FROM account_memory");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_AccountCollectedMemoryMap.insert(std::pair<int32, uint32>(fields[0].GetInt32(), fields[1].GetUInt32()));
        } while (result->NextRow());
    }

    m_MemoryCodeMap.clear();
    result = CharacterDatabase.Query("SELECT Code, Memory, MemoryLeft, Item, ItemCount FROM memory_code");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            MemoryCode memory;
            memory.memory = fields[1].GetUInt32();
            memory.count = fields[2].GetInt32();
            memory.item = fields[3].GetUInt32();
            memory.itemCount = fields[4].GetUInt32();
            m_MemoryCodeMap.insert(std::pair<std::string, MemoryCode>(fields[0].GetString(), memory));
        } while (result->NextRow());
    }
}

const CollectableMemory* MemoryMgr::GetCollectableMemory(uint32 entry) const
{
    CollectableMemoryMap::const_iterator itr = m_CollectableMemoryMap.find(entry);
    if (itr != m_CollectableMemoryMap.end())
        return &itr->second;
    return NULL;
}

void MemoryMgr::CollectAccountMemory(Player* player)
{
    // overall account memory
    AccountCollectableMemoryConstBounds gloBound = GetAccountCollectableMemoryConstBounds(-1);
    if (gloBound.first != gloBound.second)
    {
        for (AccountCollectedMemoryMap::const_iterator itr = gloBound.first; itr != gloBound.second; ++itr)
        {
            if (!player->MemoryCollected(itr->second))
            {
                const CollectableMemory* memory = GetCollectableMemory(itr->second);
                if (memory)
                    player->CollectMemory(memory, false);
            }
        }
    }

    // account memory
    uint32 account = player->GetSession()->GetAccountId();
    AccountCollectableMemoryConstBounds accBound = GetAccountCollectableMemoryConstBounds(account);
    if (accBound.first != accBound.second)
    {
        for (AccountCollectedMemoryMap::const_iterator itr = accBound.first; itr != accBound.second; ++itr)
        {
            if (!player->MemoryCollected(itr->second))
            {
                const CollectableMemory* memory = GetCollectableMemory(itr->second);
                if (memory)
                    player->CollectMemory(memory, false);
            }
        }
    }
}

AccountCollectableMemoryConstBounds MemoryMgr::GetAccountCollectableMemoryConstBounds(uint32 account) const
{
    return m_AccountCollectedMemoryMap.equal_range(account);
}

void MemoryMgr::FetchMemoryFromCode(Player* player, std::string code)
{
    MemoryCodeMap::iterator itr = m_MemoryCodeMap.find(code);
    if (itr != m_MemoryCodeMap.end())
    {
        if (player->MemoryCollected(itr->second.memory))
        {
            ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(61).c_str());
            return;
        }

        if (itr->second.count)
        {
            if (itr->second.count != -1)
            {
                --itr->second.count;
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MEMORY_CODE);
                stmt->setUInt32(0, itr->second.memory);
                stmt->setInt32(1, itr->second.count);
                stmt->setString(2, code);
                stmt->setUInt32(3, player->GetGUID()); // only record last collector
                CharacterDatabase.Execute(stmt);
            }

            if (const CollectableMemory* memory = GetCollectableMemory(itr->second.memory))
                player->CollectMemory(memory);

            if (itr->second.item)
            {
                ItemPosCountVec sDest;
                // store in main bag to simplify second pass (special bags can be not equipped yet at this moment)
                InventoryResult msg = player->CanStoreNewItem(INVENTORY_SLOT_BAG_0, NULL_SLOT, sDest, itr->second.item, itr->second.itemCount);
                if (msg == EQUIP_ERR_OK)
                    player->StoreNewItem(sDest, itr->second.item, true, Item::GenerateItemRandomPropertyId(itr->second.item));
                else
                {
                    SQLTransaction trans = CharacterDatabase.BeginTransaction();
                    MailDraft draft = MailDraft(301);
                    Item* item = Item::CreateItem(itr->second.item, itr->second.itemCount, player);
                    if (item)
                    {
                        item->SaveToDB(trans);
                        draft.AddItem(item);
                    }
                    draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, 43304));
                    CharacterDatabase.CommitTransaction(trans);
                }

                std::string name = ItemChatLink::FormatName(itr->second.item);
                ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(58, name.c_str(), itr->second.itemCount).c_str());
            }

            return;
        }
    }

    ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(59).c_str());
}

void MemoryMgr::AddMemoryCode(std::string code, uint32 memory, uint32 count, uint32 item, uint32 itemCount, std::string comment)
{
    MemoryCode memCode;
    memCode.memory = memory;
    memCode.count = count;
    memCode.item = item;
    memCode.itemCount = itemCount;
    m_MemoryCodeMap[code] = memCode;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_MEMORY_CODE);
    stmt->setString(0, code);
    stmt->setUInt32(1, memory);
    stmt->setUInt32(2, count);
    stmt->setUInt32(3, item);
    stmt->setUInt32(4, itemCount);
    stmt->setUInt32(5, 0);
    stmt->setString(6, comment);
    CharacterDatabase.Execute(stmt);
}
