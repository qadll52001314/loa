#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Player.h"

struct CollectableMemory
{
    uint32 id;
    uint32 reqItem;
    uint32 reqItemCount;
    uint32 rewardItem;
    uint32 rewardItemCount;
    uint32 rewardSpell;
    uint32 mail;
    uint32 sender;
    uint32 text;
};

struct MemoryCode
{
    uint32 memory;
    int32 count;
    uint32 item;
    uint32 itemCount;
};

typedef std::map<uint32, CollectableMemory> CollectableMemoryMap;
typedef std::multimap<int32, uint32> AccountCollectedMemoryMap;
typedef std::pair<AccountCollectedMemoryMap::const_iterator, AccountCollectedMemoryMap::const_iterator> AccountCollectableMemoryConstBounds;
typedef std::map<std::string, MemoryCode> MemoryCodeMap;

class MemoryMgr
{
public:
    MemoryMgr() {};
    ~MemoryMgr() {};
    static MemoryMgr* instance()
    {
        static MemoryMgr instance;
        return &instance;
    }

    void LoadDefine();
    const CollectableMemory* GetCollectableMemory(uint32 entry) const;
    void CollectAccountMemory(Player* player);
    AccountCollectableMemoryConstBounds GetAccountCollectableMemoryConstBounds(uint32 account) const;
    void FetchMemoryFromCode(Player* player, std::string code);
    void AddMemoryCode(std::string code, uint32 memory, uint32 count, uint32 item, uint32 itemCount, std::string comment);

private:
    CollectableMemoryMap m_CollectableMemoryMap;
    AccountCollectedMemoryMap m_AccountCollectedMemoryMap;
    MemoryCodeMap m_MemoryCodeMap;
};

#define xMemoryMgr MemoryMgr::instance()

#endif
