#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "json.hpp"
#include <cstring>
using namespace std;
using json = nlohmann::json;
string secret = "PBG892FXX982ABC*";
bool isValidUTF8(const string& str) {
    int len = str.length();
    int i = 0;
    while (i < len) {
        unsigned char c = str[i];
        if (c <= 0x7f) {
            i++;
        } else if (c >= 0xc0 && c <= 0xdf) {
            if (i + 1 < len && (str[i + 1] & 0xc0) == 0x80) {
                i += 2;
            } else {
                return false;
            }
        } else if (c >= 0xe0 && c <= 0xef) {
            if (i + 2 < len && (str[i + 1] & 0xc0) == 0x80 && (str[i + 2] & 0xc0) == 0x80) {
                i += 3;
            } else {
                return false;
            }
        } else if (c >= 0xf0 && c <= 0xf7) {
            // 4-byte character
            if (i + 3 < len && (str[i + 1] & 0xc0) == 0x80 && (str[i + 2] & 0xc0) == 0x80 && (str[i + 3] & 0xc0) == 0x80) {
                i += 4;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

int main() {
    json jdata;
    std::ifstream file("items.dat", std::ios::binary | std::ios::ate);
    int size = file.tellg();
    if (size == -1) {
        cout << "Failed to open file, are you sure that you have items.dat in current directory?" << std::endl;
        return 0;
    }
    char* data = new char[size];
    file.seekg(0, std::ios::beg);

    if (file.read((char*)(data), size)) {
        cout << "Decoding items data..." << endl;
    } else {
        cout << "Decoding of items data has failed..." << endl;
        delete[] data;
        return 0;
    }

    int itemCount;
    int memPos = 0;
    int16_t itemsdatVersion = 0;
    memcpy(&itemsdatVersion, data + memPos, 2);
    memPos += 2;
    memcpy(&itemCount, data + memPos, 4);
    memPos += 4;
    jdata["itemsdatVersion"] = itemsdatVersion;
    jdata["itemCount"] = itemCount;
    cout << "itemCount: " << itemCount << endl;

    for (int i = 0; i < itemCount; i++) {
        json j;
        cout << "Processing Item " << i + 1 << " of " << itemCount << endl;

        int itemID = 0;
        char editableType = 0;
        char itemCategory = 0;
        char actionType = 0;
        char hitSoundType = 0;
        string name = "";
        string texture = "";
        int textureHash = 0;
        char itemKind = 0;
        int val1;
        char textureX = 0;
        char textureY = 0;
        char spreadType = 0;
        char isStripeyWallpaper = 0;
        char collisionType = 0;
        unsigned char breakHits = 0;
        int dropChance = 0;
        char clothingType = 0;
        int16_t rarity = 0;
        unsigned char maxAmount = 0;
        string extraFile = "";
        int extraFileHash = 0;
        int audioVolume = 0;
        string petName = "";
        string petPrefix = "";
        string petSuffix = "";
        string petAbility = "";
        char seedBase = 0;
        char seedOverlay = 0;
        char treeBase = 0;
        char treeLeaves = 0;
        int seedColor = 0;
        int seedOverlayColor = 0;
        int growTime = 0;
        short val2;
        short isRayman = 0;
        string extraOptions = "";
        string texture2 = "";
        string extraOptions2 = "";
        string punchOptions = "";

        memcpy(&itemID, data + memPos, 4);
        memPos += 4;

        editableType = data[memPos];
        memPos += 1;
        itemCategory = data[memPos];
        memPos += 1;
        actionType = data[memPos];
        memPos += 1;
        hitSoundType = data[memPos];
        memPos += 1;

        int16_t strLen = *(int16_t*)&data[memPos];
        memPos += 2;
        if (strLen <= 0 || strLen > 256) {
            cout << "Skipping item due to invalid name length at Item " << i + 1 << endl;
            continue;
        }

        for (int j = 0; j < strLen; j++) {
            name += data[memPos] ^ (secret[(j + itemID) % secret.length()]);
            memPos++;
        }

        if (!isValidUTF8(name)) {
            cout << "Skipping item due to invalid UTF-8 name at Item " << i + 1 << endl;
            continue;  // Skip this item
        }

        strLen = *(int16_t*)&data[memPos];
        memPos += 2;
        if (strLen <= 0 || strLen > 1024) {
            cout << "Skipping item due to invalid texture length at Item " << i + 1 << endl;
            continue;
        }

        for (int j = 0; j < strLen; j++) {
            texture += data[memPos];
            memPos++;
        }

        if (!isValidUTF8(texture)) {
            cout << "Skipping item due to invalid UTF-8 texture at Item " << i + 1 << endl;
            continue; 
        }

        memcpy(&textureHash, data + memPos, 4);
        memPos += 4;

        j["itemID"] = itemID;
        j["name"] = name;
        j["texture"] = texture;
        j["textureHash"] = textureHash;
        j["editableType"] = editableType;
        j["itemCategory"] = itemCategory;
        j["actionType"] = actionType;
        j["hitSoundType"] = hitSoundType;
        jdata["items"].push_back(j);

        cout << "Finished processing item " << i + 1 << endl;
    }

    cout << "Saving data..." << endl;
    ofstream o("data.json");
    o << setw(4) << jdata << endl;
    cout << "Successfully decoded" << endl;

    delete[] data;
    return 0;
}
