/**
 * @file main.cpp
 * @author Hayden Lauritzen (haydenlauritzen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lib/DataManager/datamanager.cpp"
#include <algorithm>

const std::string FILEID = "dex";

static PokemonManager pm;
static std::map<std::string, Pokemon> dex;

void init() {
    dex = pm.read(FILEID);
}

void editDex() {
    std::string editInput;
    std::string name, levelUp;
    Pokemon::xpCurve curve;
    int dexNum;

    std::cout << "NatDex Number: [int]" << std::endl;
    std::cin >> dexNum;

    std::cout << "Pokemon Name: [string]" << std::endl;
    std::cin >> name;

    std::cout << "XP Curve [Erratic = 5, Fast = 4, MediumFast = 3, MediumSlow = 2, Slow = 1, Fluctuating = 0]"
              << std::endl;
    int levelVal;
    std::cin >> levelVal;
    curve = Pokemon::xpCurve(levelVal);

    std::cout << "When does/how it level up [int/string]" << std::endl;
    std::cin >> levelUp;

    std::cout << "Is [" << dexNum << "] " << name << " " << curve << " @ level " << levelUp << " Correct? [y/n]" << std::endl;
    std::cin >> editInput;
    if(editInput.at(0) != 'y') editDex();

    Pokemon p(dexNum, name, curve, levelUp);
    dex.insert(std::make_pair(name, p));
    pm.write(dex, FILEID);
}

void run() {

    const char exit = 'x';
    const char edit = 'e';
    const char calc = 'c';
    std::string input;

    do {
        std::cout << "Exit [x], Edit [e], Calc [c]" << std::endl;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        // grab first char
        char i = input.at(0);
        switch(i) {
        case exit:
            return;
        case edit: {
            editDex();
            break;
        }
        case calc: {
            break;
        }
        }

    } while(input.at(0) != exit);
}

int main() {

    init();
    run();

    std::cout << "Closing" << std::endl;
}