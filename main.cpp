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
#include <cmath>

const std::string FILEID = "dex";

static PokemonManager pm;
static std::map<std::string, Pokemon> dex;

void init() {
    dex = pm.read(FILEID);
}

float constexpr erratic(int level) {
    if(level < 0 || level > 100) return -1;
    if(level < 50)
        return ((float)(level ^ 3) * (100 - level)) / 50;
    else if(level < 68)
        return ((float)(level ^ 3) * (150 - level)) / 50;
    else if(level < 98)
        return (level ^ 3) * std::floor((1911 - (float)(10 * level)) / 3);
    else // >= 98 && <= 100
        return ((float)(level ^ 3) * (160 - level)) / 100;
}

float constexpr fast(int level) {
    if(level < 0 || level > 100) return -1;
    return ((float)4 * (level ^ 3)) / 5;
}

float constexpr mediumFast(int level) {
    if(level < 0 || level > 100) return -1;
    return (float)(level ^ 3);
}

float constexpr mediumSlow(int level) {
    if(level < 0 || level > 100) return -1;
    return (float)(6 / 5) * (level ^ 3) - 15 * (level ^ 2) + 100 * level - 140;
}

float constexpr slow(int level) {
    if(level < 0 || level > 100) return -1;
    return (float)(5 / 4) * (level ^ 3);
}

float constexpr fluctuating(int level) {
    if(level < 0 || level > 100) return -1;
    if(level < 15)
        return ((float)(level ^ 3) * std::floor((float)(level + 1) / 3) + 24) / 50;
    else if(level < 36)
        return ((float)(level ^ 3) * (level + 14)) / 50;
    else // >= 36 && <= 100
        return ((float)(level ^ 3) * std::floor((float)(level) / 2) + 32) / 50;
}

void calcLevel() {

    std::string input;
    if(input == "x" || input == "exit") return;
    std::cout << "Pokemon Name?" << std::endl;
    std::cin >> input;

    auto it = dex.find(input);
    if(it == dex.end()) {
        std::cout << "Invalid Name or Pokemon not in Dex" << std::endl;
        return;
    }

    Pokemon p = it->second;
    Pokemon::xpCurve curve = p.curve;

    std::cout << "Starting Level?" << std::endl;
    std::cin >> input;
    int start = std::stoi(input); // try/catch
    std::cout << "Current Exp?" << std::endl;
    std::cin >> input;
    int initExp = std::stoi(input);
    std::cout << "Desired Level or Evolve [e]" << std::endl;
    std::cin >> input;
    int end;
    if(input.at(0) == 'e') {
        try {
            end = std::stoi(p.evolve);
        } catch(std::exception e) {
            std::cout << "Pokemon evolves by: " << p.evolve << std::endl;
        }
    } else {
        end = std::stoi(input);
    }

    if(start < 0 || start > 100 || end < 0 || end > 100) {
        std::cout << "Invalid level" << std::endl;
    }
    float expReq = 0.0f;
    switch(curve) {
    case(Pokemon::xpCurve::Erratic): {
        expReq = erratic(end) - erratic(start) - initExp;
        break;
    }
    case(Pokemon::xpCurve::Fast): {
        expReq = fast(end) - fast(start) - initExp;
        break;
    }
    case(Pokemon::xpCurve::MediumSlow): {
        expReq = mediumSlow(end) - mediumSlow(start) - initExp;
        break;
    }
    case(Pokemon::xpCurve::MediumFast): {
        expReq = mediumFast(end) - mediumFast(start) - initExp;
        break;
    }
    case(Pokemon::xpCurve::Slow): {
        expReq = slow(end) - slow(start) - initExp;
        break;
    }
    case(Pokemon::xpCurve::Fluctuating): {
        expReq = fluctuating(end) - fluctuating(start) - initExp;
        break;
    }
    }

    std::cout << "Pokemon needs " << expReq << " exp." << std::endl;
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
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    std::cout << "XP Curve [Erratic = 5, Fast = 4, MediumFast = 3, MediumSlow = 2, Slow = 1, Fluctuating = 0]"
              << std::endl;
    int levelVal;
    std::cin >> levelVal;
    curve = Pokemon::xpCurve(levelVal);

    std::cout << "When does/how it level up [int/string]" << std::endl;
    std::cin >> levelUp;

    std::cout << "Is [" << dexNum << "] " << name << " " << curve << " @ level " << levelUp << " Correct? [y/n]"
              << std::endl;
    std::cin >> editInput;
    if(editInput.at(0) != 'y') return;


    Pokemon p(dexNum, name, curve, levelUp);
    dex[name] = p;
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
            std::cout << "[x/exit] to exit." << std::endl;
            calcLevel();
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