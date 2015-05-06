#include "Chat.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "Player.h"
#include "Pet.h"

class legacy_commandscript : public CommandScript
{
public:
    legacy_commandscript() : CommandScript("legacy_commandscript") {}

    ChatCommand* GetCommands() const override
    {
        static ChatCommand addCommandTable[] =
        {
            { "memorycode", rbac::RBAC_PERM_COMMAND_LEGACY_ADD_MEMORY_CODE, true, &HandleAddMemoryCode, "", NULL },
            { "memory", rbac::RBAC_PERM_COMMAND_LEGACY_ADD_MEMORY, true, &HandleAddMemory, "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };

        static ChatCommand legacyCommandTable[] =
        {
            { "add", rbac::RBAC_PERM_COMMAND_LEGACY_ADD, false, NULL, "", addCommandTable },
            { "", rbac::RBAC_PERM_COMMAND_LEGACY, false, &HandleLegacyCommand, "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "legacy", rbac::RBAC_PERM_COMMAND_LEGACY, false, NULL, "", legacyCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }

    static bool HandleAddMemoryCode(ChatHandler* handler, char const* args)
    {
        // format: 
        return true;
    }

    static bool HandleAddMemory(ChatHandler* handler, char const* args)
    {
        return true;
    }

    static bool HandleLegacyCommand(ChatHandler* handler, char const* args)
    {
        return true;
    }
};

void AddSC_legacy_commandscript()
{
    new legacy_commandscript();
}