#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Player.h"

struct CollectableMemory
{
    uint32 id;
    uint32 item;
    uint32 itemCount;
    uint32 spell;
    uint32 mail;
    uint32 sender;
};

struct CharacterCollectableMemory
{
    uint32 memory;
    bool collected;
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
typedef std::multimap<int32, CharacterCollectableMemory> CharacterCollectedMemoryMap;
typedef std::pair<AccountCollectedMemoryMap::const_iterator, AccountCollectedMemoryMap::const_iterator> AccountCollectableMemoryConstBounds;
typedef std::pair<CharacterCollectedMemoryMap::const_iterator, CharacterCollectedMemoryMap::const_iterator> CharacterCollectableMemoryConstBounds;
typedef std::pair<CharacterCollectedMemoryMap::iterator, CharacterCollectedMemoryMap::iterator> CharacterCollectableMemoryBounds;
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
    void CollectMemory(Player* player);
    void CollectMemory(uint32 character, uint32 memory);
    bool CharacterCollected(uint32 character, uint32 memory) const;
    AccountCollectableMemoryConstBounds GetAccountCollectableMemoryConstBounds(uint32 account) const;
    CharacterCollectableMemoryConstBounds GetCharacterCollectableMemoryConstBounds(uint32 character) const;
    CharacterCollectableMemoryBounds GetCharacterCollectableMemoryBounds(uint32 character);
    void FetchMemoryFromCode(Player* player, std::string code);
    void SendMemoryMail(Player* player, const CollectableMemory* memory) const;

private:
    CollectableMemoryMap m_CollectableMemoryMap;
    AccountCollectedMemoryMap m_AccountCollectedMemoryMap;
    CharacterCollectedMemoryMap m_CharacterCollectedMemoryMap;
    MemoryCodeMap m_MemoryCodeMap;
};

#define xMemoryMgr MemoryMgr::instance()

#endif
