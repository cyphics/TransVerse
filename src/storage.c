/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Thierry Raeber$
   ======================================================================== */

#include <stdlib.h>
#include "storage.h"
#include "helper.h"

char *GetXmlSegment(char receiver[], char xml_code[], char *entry)
{
    /*
      Saves the content of the next xml child named 'entry'
      in the receiver.
      Returns a char* pointer located after that entry, if it was found.
      Returns NULL otherwise.
    */

    receiver[0] = '\0';
    char open_str[20], close_str[20];
    sprintf(open_str, "<%s>", entry);
    sprintf(close_str, "</%s>", entry);

    char *start = strstr(xml_code,  open_str);
    if (!start) {
        return NULL    ;
    }
    start += strlen(open_str);
    while(*start == '\n') start++;
    char *end = strstr(start,  close_str);
    strncpy(receiver, start, end-start);
    end += strlen(close_str);
    // if (strcmp(entry, "id") == 0) {
    //     printf("id: %s\n", end);
    // }
    return end;
}

price BuildPrice(char *xml_code)
{
    price p = {};

    if (strcmp(xml_code, "") != 0) {
        resource res_list[MAX_RESOURCES_PER_PRICE] = {};
        LoadResources(xml_code, res_list);
        memcpy(p.resources, res_list, sizeof(res_list));
    }

    return p;
}

upgrade BuildUpgrade(char *xml_code)
{
    char id[50] = {};
    char type[20] = {};
    char desc[100] = {};
    char increase_factor_str[3] = {};
    char price_str[100] = {};
    char xml_dep[200] = {};
    char dep_list[MAX_DEPENDENCIES][30] = {};

    GetXmlSegment(id, xml_code, "id");
    GetXmlSegment(type, xml_code, "type");
    GetXmlSegment(desc, xml_code, "desc");
    GetXmlSegment(increase_factor_str, xml_code, "inc");
    GetXmlSegment(price_str, xml_code, "Price");
    GetXmlSegment(xml_dep, xml_code, "Dependencies");
    char *next_dep = xml_dep;
    int i = 0;

    while(next_dep != NULL) {
        char cur_dep[30] = {};
        next_dep = GetXmlSegment(cur_dep, next_dep, "Dependency");
        if (next_dep != NULL) {
            memcpy(dep_list[i], cur_dep, sizeof(cur_dep));
            i++;
        }

    }

    // if (!AreStrEquals(xml_dep[i], "")) {
    //     memcpy(dep_list[i], xml_dep[i], sizeof(xml_dep[i]));
    // }


    price init_price = BuildPrice(price_str);
    float inc_factor = atof(increase_factor_str);

    upgrade u = {};
    strcpy(u.id, id);
    strcpy(u.type, type);
    strcpy(u.description, desc);
    memcpy(u.dependencies, dep_list, sizeof(dep_list));
    u.initial_price = init_price;
    u.increase_factor = inc_factor;

    return u;
}

resource BuildResource(char *xml_code)
{
    char type[20] = {};
    char amount_str[50] = {};
    int amount = 0;

    GetXmlSegment(type, xml_code, "type");
    GetXmlSegment(amount_str, xml_code, "amount");

    amount = strtol(amount_str, NULL, 10);

    resource res = {};
    strcpy(res.type, type);
    res.amount = amount;
    return res;

}

void PriceToStr(char *ptr, price p)
{
    strcat(ptr, "<Price>\n");
    for (int i = 0; i < MAX_RESOURCES_PER_PRICE; i++) {
        char res_str[150] = {};
        resource res = p.resources[i];
        if (strcmp(res.type, "") != 0) {
            sprintf(res_str, "<Resource>\n<type>%s</type>\n<amount>%d</amount>\n</Resource>\n",
                    res.type,
                    res.amount);
            strcat(ptr, res_str);
        }
    }
    strcat(ptr, "</Price>\n");
}

void DependencyToStr(char *ptr, char deps[MAX_DEPENDENCIES][30])
{
    strcat(ptr, "<Dependencies>\n");
    for (int i = 0; i < MAX_DEPENDENCIES; i++) {
        if (!IsEmpty(deps[i]) ) {
            strcat(ptr, "<Dependency>");
            strcat(ptr, deps[i]);
            strcat(ptr, "</Dependency>\n");
        }
    }
    strcat(ptr, "</Dependencies>\n");
}

void SaveUpgrades(char *output, upgrade *up_list)
{
    strcat(output, "<Upgrades>\n\n");

    for (int i = 0; i < MAX_UPGRADES_AMOUNT; i++)
    {
        upgrade curr_up = up_list[i];
        char price_str[100] = {};
        char deps_str[120] = {};
        PriceToStr(price_str, curr_up.initial_price);
        DependencyToStr(deps_str, curr_up.dependencies);
        if (strcmp(curr_up.type, "") != 0) {
            char up_str[350] = {};
            sprintf(up_str, "<Upgrade>\n<id>%s</id>\n<type>%s</type>\n<desc>%s</desc>\n%s<bought>%d</bought>\n<inc>%.2g</inc>\n%s</Upgrade>\n\n",
                    curr_up.id,
                    curr_up.type,
                    curr_up.description,
                    price_str,
                    curr_up.amount_bought,
                    curr_up.increase_factor,
                    deps_str);
            strcat(output, up_str);
        }
    }
    strcat(output, "</Upgrades>\n\n");
}

void SaveResources(char *output, resource *res_list)
{
    strcat(output, "<Resources>\n");
    resource *curr_res;
    for (int i = 0; i < MAX_RESOURCES_AMOUNT; i++)
    {
        curr_res = &res_list[i];
        if (strcmp(curr_res->type, "") != 0) {
            char res_str[100] = {};
            sprintf(res_str, "<Resource>\n<type>%s</type>\n<amount>%d</amount>\n</Resource>\n",
                    curr_res->type,
                    curr_res->amount);
            strcat(output, res_str);
        }
    }
    strcat(output, "</Resources>\n");
}
void LoadUpgrades(char *xml_code, upgrade *upgrades_list)
{
    char *next = xml_code;
    int idx = 0;
    while (next != NULL) {
        char next_upgrade[300] = {};
        next = GetXmlSegment(next_upgrade, next, "Upgrade");
        if (next != NULL) {
            upgrade u = BuildUpgrade(next_upgrade);
            upgrades_list[idx] = u;
            idx++;
        }
    }
}

void LoadResources(char *xml_code, resource *resources_list)
{
    char *next = xml_code;
    int idx = 0;
    while (next != NULL) {
        char next_resource[100] = {};
        next = GetXmlSegment(next_resource, next, "Resource");
        if (next != NULL) {
            resource res = BuildResource(next_resource);
            resources_list[idx] = res;
            idx++;
        }
    }
}

bool LoadGameFromFile(char *file_path, GameState *state)
{
    FILE *fptr = fopen(file_path, "r");
    if (fptr == NULL) {
        printf ("File %s failed to open\n", file_path);
        return false;
    }

    char *buffer = 0;
    upgrade upgrades_list[MAX_UPGRADES_AMOUNT] = {};
    resource resources_list[MAX_RESOURCES_AMOUNT] = {};

    fseek(fptr, 0, SEEK_END);
    long length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    buffer = (char*)malloc(length);
    if (buffer) {
        fread(buffer, 1, length, fptr);
    } else return false;
    fclose(fptr);

    if (buffer) {
        char upgrades_code[5000] = {};
        GetXmlSegment(upgrades_code, buffer, "Upgrades");
        LoadUpgrades(upgrades_code, upgrades_list);
        char resources_code[5000] = {};
        GetXmlSegment(resources_code, buffer, "Resources");
        LoadResources(resources_code, resources_list);
    } else return false;

    memcpy(state->upgrades_list, upgrades_list, sizeof(upgrade) * MAX_UPGRADES_AMOUNT);
    memcpy(state->resources_list, resources_list, sizeof(resource) * MAX_RESOURCES_AMOUNT);
    return true;
}

void SaveGameToDisk(char *file_name, GameState *state)
{
    FILE *fptr = fopen(file_name, "w");
    if (fptr == NULL) {
        printf ("File failed to open\n");
        return;
    }

    char *xml_header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    char upgrades_xml[10000] = {};
    char resources_xml[10000] = {};
    SaveUpgrades(upgrades_xml, state->upgrades_list);
    SaveResources(resources_xml, state->resources_list);
    char final_xml[sizeof(xml_header) + sizeof(upgrades_xml) + sizeof(resources_xml)] = {};
    strcat(final_xml, xml_header);
    strcat(final_xml, upgrades_xml);
    strcat(final_xml, resources_xml);
    fprintf(fptr, final_xml);
    fclose(fptr);
}
