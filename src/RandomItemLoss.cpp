#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Random.h"

class RandomItemLossPlayerScript : public PlayerScript
{
public:
    RandomItemLossPlayerScript() : PlayerScript("RandomItemLossPlayerScript") { }

    void OnKilledByCreature(Player* victim, Creature* killer) override
    {
        HandlePlayerDeath(victim);
    }

    void OnKilledByPlayer(Player* victim, Player* killer) override
    {
        HandlePlayerDeath(victim);
    }

    void HandlePlayerDeath(Player* player)
    {
        if (!sConfigMgr->GetOption<bool>("RandomItemLoss.Enabled", true))
            return;

        std::vector<uint8> equippedSlots;

        // Loop over EQUIPMENT_SLOT_START to EQUIPMENT_SLOT_END
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                equippedSlots.push_back(slot);
        }

        if (equippedSlots.empty())
            return;

        // Select a random equipped slot
        uint8 randomSlot = equippedSlots[urand(0, equippedSlots.size() - 1)];
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, randomSlot);

        if (item)
        {
            player->DestroyItemCount(item->GetEntry(), 1, true);
            ChatHandler(player->GetSession()).SendSysMessage("A mysterious force has taken one of your equipped items...");
        }
    }
};

class mod_random_item_loss_on_death : public ModuleScript
{
public:
    mod_random_item_loss_on_death() : ModuleScript("mod_random_item_loss_on_death") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        sConfigMgr->LoadMoreConfig("conf/random_item_loss.conf");
    }

    void OnAllModulesLoaded() override
    {
        new RandomItemLossPlayerScript();
    }
};

void Addmod_random_item_loss_on_death()
{
    new mod_random_item_loss_on_death();
}
