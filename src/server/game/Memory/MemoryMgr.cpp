#include "MemoryMgr.h"
#include "WorldSession.h"
#include "ChatLink.h"
#include "Chat.h"

void MemoryMgr::LoadDefine()
{
    m_CollectableMemoryMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT ID, Item, ItemCount, Spell, Mail, Sender FROM collectable_memory");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CollectableMemory memory;
            memory.id = fields[0].GetUInt32();
            memory.item = fields[1].GetUInt32();
            memory.itemCount = fields[2].GetUInt32();
            memory.spell = fields[3].GetUInt32();
            memory.mail = fields[4].GetUInt32();
            memory.sender = fields[5].GetUInt32();
            m_CollectableMemoryMap.insert(std::pair<uint32, CollectableMemory>(memory.id, memory));
        } while (result->NextRow());
    }

    m_AccountCollectedMemoryMap.clear();
    result = LoginDatabase.Query("SELECT Account, Memory FROM account_collectable_memory");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            m_AccountCollectedMemoryMap.insert(std::pair<int32, uint32>(fields[0].GetInt32(), fields[1].GetUInt32()));
        } while (result->NextRow());
    }

    m_CharacterCollectedMemoryMap.clear();
    result = CharacterDatabase.Query("SELECT Guid, Memory, Collected FROM character_collectable_memory");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            CharacterCollectableMemory memory;
            memory.memory = fields[1].GetUInt32();
            memory.collected = fields[2].GetBool();
            m_CharacterCollectedMemoryMap.insert(std::pair<int32, CharacterCollectableMemory>(fields[0].GetInt32(), memory));
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

void MemoryMgr::CollectMemory(Player* player)
{
    // character memory
    uint32 character = player->GetGUID();
    CharacterCollectableMemoryBounds chrBound = GetCharacterCollectableMemoryBounds(character);
    if (chrBound.first != chrBound.second)
    {
        CharacterCollectedMemoryMap::iterator itr = chrBound.first;
        while (itr != chrBound.second)
        {
            if (!itr->second.collected)
            {
                const CollectableMemory* memory = GetCollectableMemory(itr->second.memory);
                if (memory)
                {
                    if (memory->item)
                    {
                        SQLTransaction trans = CharacterDatabase.BeginTransaction();
                        MailDraft draft = MailDraft(memory->mail);
                        Item* item = Item::CreateItem(memory->item, memory->itemCount, player);
                        if (item)
                        {
                            item->SaveToDB(trans);
                            draft.AddItem(item);
                        }
                        draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, memory->sender));
                        CharacterDatabase.CommitTransaction(trans);
                    }

                    if (memory->spell)
                        player->LearnSpell(memory->spell, false);

                    //CollectMemory(character, itr->second.memory);
                    itr->second.collected = true;
                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_COLLECTABLE_MEMORY);
                    stmt->setUInt32(0, character);
                    stmt->setUInt32(1, itr->second.memory);
                    stmt->setBool(2, true);
                    CharacterDatabase.Execute(stmt);
                }
            }

            ++itr;
        }
    }

    // overall memory
    AccountCollectableMemoryConstBounds gloBound = GetAccountCollectableMemoryConstBounds(-1);
    if (gloBound.first != gloBound.second)
    {
        for (AccountCollectedMemoryMap::const_iterator itr = gloBound.first; itr != gloBound.second; ++itr)
        {
            if (!CharacterCollected(character, itr->second))
            {
                const CollectableMemory* memory = GetCollectableMemory(itr->second);
                if (memory)
                {
                    if (memory->item)
                    {
                        SQLTransaction trans = CharacterDatabase.BeginTransaction();
                        MailDraft draft = MailDraft(memory->mail);
                        Item* item = Item::CreateItem(memory->item, memory->itemCount, player);
                        if (item)
                        {
                            item->SaveToDB(trans);
                            draft.AddItem(item);
                        }
                        draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, memory->sender));
                        CharacterDatabase.CommitTransaction(trans);
                    }

                    if (memory->spell)
                        player->LearnSpell(memory->spell, false);

                    CollectMemory(character, itr->second);
                }
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
            if (!CharacterCollected(character, itr->second))
            {
                const CollectableMemory* memory = GetCollectableMemory(itr->second);
                if (memory)
                {
                    if (memory->item)
                    {
                        SQLTransaction trans = CharacterDatabase.BeginTransaction();
                        MailDraft draft = MailDraft(memory->mail);
                        Item* item = Item::CreateItem(memory->item, memory->itemCount, player);
                        if (item)
                        {
                            item->SaveToDB(trans);
                            draft.AddItem(item);
                        }
                        draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, memory->sender));
                        CharacterDatabase.CommitTransaction(trans);
                    }

                    if (memory->spell)
                        player->LearnSpell(memory->spell, false);

                    CollectMemory(character, itr->second);
                }
            }
        }
    }
}

void MemoryMgr::CollectMemory(uint32 character, uint32 memory)
{
    CharacterCollectableMemoryBounds bound = GetCharacterCollectableMemoryBounds(character);
    if (bound.first != bound.second)
    {
        for (CharacterCollectedMemoryMap::iterator itr = bound.first; itr != bound.second; ++itr)
        {
            if (itr->second.memory == memory)
            {
                itr->second.collected = true;
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_COLLECTABLE_MEMORY);
                stmt->setUInt32(0, character);
                stmt->setUInt32(1, memory);
                stmt->setBool(2, true);
                CharacterDatabase.Execute(stmt);
                return;
            }
        }
    }

    CharacterCollectableMemory memo;
    memo.memory = memory;
    memo.collected = true;
    m_CharacterCollectedMemoryMap.insert(std::pair<uint32, CharacterCollectableMemory>(character, memo));

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_COLLECTABLE_MEMORY);
    stmt->setUInt32(0, character);
    stmt->setUInt32(1, memory);
    stmt->setBool(2, true);
    CharacterDatabase.Execute(stmt);
}

AccountCollectableMemoryConstBounds MemoryMgr::GetAccountCollectableMemoryConstBounds(uint32 account) const
{
    return m_AccountCollectedMemoryMap.equal_range(account);
}

CharacterCollectableMemoryConstBounds MemoryMgr::GetCharacterCollectableMemoryConstBounds(uint32 character) const
{
    return m_CharacterCollectedMemoryMap.equal_range(character);
}

CharacterCollectableMemoryBounds MemoryMgr::GetCharacterCollectableMemoryBounds(uint32 character)
{
    return m_CharacterCollectedMemoryMap.equal_range(character);
}

bool MemoryMgr::CharacterCollected(uint32 character, uint32 memory) const
{
    CharacterCollectableMemoryConstBounds bound = GetCharacterCollectableMemoryConstBounds(character);
    if (bound.first == bound.second)
        return false;

    for (CharacterCollectedMemoryMap::const_iterator itr = bound.first; itr != bound.second; ++itr)
    {
        if (itr->second.memory == memory && itr->second.collected)
            return true;
    }

    return false;
}

void MemoryMgr::FetchMemoryFromCode(Player* player, std::string code)
{
    MemoryCodeMap::iterator itr = m_MemoryCodeMap.find(code);
    if (itr != m_MemoryCodeMap.end())
    {
        // @todo: finish '1 fetch per player' function
        if (itr->second.count)
        {
            if (itr->second.count != -1)
            {
                --itr->second.count;
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MEMORY_CODE);
                stmt->setUInt32(0, itr->second.memory);
                stmt->setInt32(1, itr->second.count);
                stmt->setString(2, code);
                CharacterDatabase.Execute(stmt);
            }

            if (const CollectableMemory* memory = GetCollectableMemory(itr->second.memory))
            {
                SQLTransaction trans = CharacterDatabase.BeginTransaction();
                MailDraft draft = MailDraft(memory->mail);
                Item* item = Item::CreateItem(memory->item, memory->itemCount, player);
                if (item)
                {
                    item->SaveToDB(trans);
                    draft.AddItem(item);
                }
                draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, memory->sender));
                CharacterDatabase.CommitTransaction(trans);

                ChatHandler(player->GetSession()).SendSysMessage(sObjectMgr->GetServerMessage(60).c_str());
            }

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
