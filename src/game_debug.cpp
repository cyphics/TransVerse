/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

const static upgrade u1 = { "Terminal", "structure", "another desc", {{"energy", 10}}, 1.2, 0, {} };
const static upgrade u2 = { "Quantum Generator", "science", "a desc", {}, 1.2, 0, {"Terminal"} };
const static upgrade u3 = { "Quantum Throttle", "structure", "blah", {}, 1.4, 0, {"Terminal", "Quantum Generator"} };
const static upgrade u4 = { "Magnet", "structure", "blah", {}, 1.4, 0, "" };
const static upgrade u5 = { "GPS Firmware", "software", "blah", {}, 1.4, 0, "" };
const static upgrade u6 = { "Kinetic pusher firmware", "software", "blah", {}, 1.4, 0, "" };
const static upgrade u7 = { "Quantum Magnetism", "science", "blah", {}, 1.4, 0, "" };
const static upgrade u8 = { "Quantum Expulsion", "science", "blah", {}, 1.4, 0, "" };
const static upgrade u9 = { "Kinetic Power", "structure", "blah", {}, 1.4, 0, "" };
const static upgrade u10 = { "Quantic Coil", "incremental", "blah", {}, 1.4, 0, "" };
const static upgrade u11 = { "Quantum Joule Cell", "incremental", "blah", {}, 1.4, 0, "" };
const static upgrade u12 = { "Quantum Conductor", "structure", "blah", {}, 1.4, 0, "" };
const static upgrade u13 = { "Quantum Synergy", "science", "blah", {}, 1.4, 0, "" };

const static resource r1 = { "Knowledge", 4 };
const static resource r2 = { "Lines of code", 100 };
const static resource r3 = { "Energy", 10 };

const static upgrade init_upgrade_list[MAX_UPGRADES_AMOUNT] = {u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13};
const static resource init_resources_list[MAX_RESOURCES_AMOUNT] = {r1, r2, r3};

extern char *CONFIG_FILE;

void CreateFirstXmlFile()
{
    GameState config = BuildGameConfig(init_upgrade_list,
                                       sizeof(init_upgrade_list) / sizeof(init_upgrade_list[0]),
                                       init_resources_list,
                                       sizeof(init_resources_list) / sizeof(init_resources_list[0]));
    SaveGameToDisk(CONFIG_FILE, &config);
}

void ResourceToString(char* ptr, resource res)
{
    sprintf(ptr, "Type: %s\nAmount: %d\n", res.type, res.amount);
}

void UpgradeToString(char* ptr, upgrade up)
{
    char price_str[200] = {};
    char deps_str[120] = {};
    price p = up.initial_price;
    for (int i = 0; i < MAX_RESOURCES_PER_PRICE; i++) {
        resource r = p.resources[i];
        if (r.amount > 0) {
            char res_str[50] = {};
            sprintf(res_str, "%s, %d,", r.type, r.amount);
            strcat(price_str, res_str);
        }
    }
    for (int i = 0; i < MAX_DEPENDENCIES; i++) {
        if (!IsEmpty(up.dependencies[i])) {
            strcat(deps_str, up.dependencies[i]);
            strcat(deps_str, "\n");
        }
    }

    sprintf(ptr, "Id: %s\nType: %s\nDesc: %s\nPrice: %s\nIncrease factor: %.2g\nBought: %d\nDependencies: %s\n", up.id, up.type, up.description, price_str, up.increase_factor, up.amount_bought, deps_str);
}


void PrintUpgrade(upgrade u)
{
    char str[200];
    UpgradeToString(str, u);
    printf("%s\n", str);
}

void PrintResource(resource res)
{
    char str[100];
    ResourceToString(str, res);
    printf("%s\n", str);
}

void PrintGameState(GameState state)
{
    printf("PrintGameState():\n");
    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++) {
        if (strcmp(state.upgrades_list[i].id, "") != 0) {
            PrintUpgrade(state.upgrades_list[i]);
        }

    }
    for (int i = 0; i < MAX_RESOURCES_AMOUNT; i++) {
        if (strcmp(state.resources_list[i].type, "") != 0) {
            PrintResource(state.resources_list[i]);
        }
    }

}

void PrintUpgradeInfo(UpgradeInfo info)
{
    printf("Name: %s\n", info.name);
    printf("Amount bought: %d\n", info.amount_bought);
}
