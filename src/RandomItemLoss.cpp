#include "Player.h"
#include "ScriptMgr.h"
#include "EquipmentMgr.h"
#include <vector>
#include <random>

class RandomItemDeleteOnDeath : public PlayerScript
{
public:
    RandomItemDeleteOnDeath() : PlayerScript("RandomItemDeleteOnDeath") { }

    void OnDeath(Player* player, Unit* /*killer*/) override
    {
        std::vector<uint8> equippedSlots;

        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_EQUIPPED, slot))
            {
                equippedSlots.push_back(slot);
            }
        }

        if (equippedSlots.empty())
            return;

        // Randomly choose one slot to delete
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, equippedSlots.size() - 1);

        uint8 randomSlot = equippedSlots[dis(gen)];

        if (Item* itemToDelete = player->GetItemByPos(INVENTORY_SLOT_EQUIPPED, randomSlot))
        {
            uint32 itemId = itemToDelete->GetEntry();
            player->DestroyItemCount(itemId, 1, true);

            ChatHandler(player->GetSession()).SendSysMessage("One of your equipped items has been destroyed due to death!");
        }
    }
};

void Addmod_random_item_delete_on_deathScripts()
{
    new RandomItemDeleteOnDeath();
}