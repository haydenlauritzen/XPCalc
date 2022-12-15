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
        return ((std::pow(level, 3)) * (100.0f - level)) / 50.0f;
    else if(level < 68)
        return ((std::pow(level, 3)) * (150.0f - level)) / 50.0f;
    else if(level < 98)
        return (std::pow(level, 3)) * std::floor((1911.0f - (10.0f * level)) / 3);
    else // >= 98 && <= 100
        return ((std::pow(level, 3)) * (160.0f - level)) / 100.0f;
}

float constexpr fast(int level) {
    if(level < 0 || level > 100) return -1;
    return (4.0f * (std::pow(level, 3))) / 5.0f;
}

float constexpr mediumFast(int level) {
    if(level < 0 || level > 100) return -1;
    return (std::pow(level, 3));
}

float constexpr mediumSlow(int level) {
    if(level < 0 || level > 100) return -1;
    return (6.0f / 5.0f) * (std::pow(level, 3)) - 15.0f * (level ^ 2) + 100.0f * level - 140.0f;
}

float constexpr slow(int level) {
    if(level < 0 || level > 100) return -1;
    return (5.0f / 4.0f) * (std::pow(level, 3));
}

float constexpr fluctuating(int level) {
    if(level < 0 || level > 100) return -1;
    if(level < 15)
        return ((std::pow(level, 3)) * std::floor((level + 1.0f) / 3.0f) + 24.0f) / 50.0f;
    else if(level < 36)
        return ((std::pow(level, 3)) * (level + 14.0f)) / 50.0f;
    else // >= 36 && <= 100
        return ((std::pow(level, 3)) * std::floor((level) / 2.0f) + 32.0f) / 50.0f;
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

    std::cout << "Current Exp?" << std::endl;
    std::cin >> input;
    int initExp = std::stoi(input);
    std::cout << "Desired Level or Evolve [e]" << std::endl;
    std::cin >> input;
    int target;
    if(input.at(0) == 'e') {
        try {
            target = std::stoi(p.evolve);
        } catch(std::invalid_argument const&) {
            std::cout << "Pokemon evolves by: " << p.evolve << std::endl;
            return;
        }
    } else {
        target = std::stoi(input);
    }

    if(target < 0 || target > 100) {
        std::cout << "Invalid level" << std::endl;
    }
    float expReq = 0.0f;
    switch(curve) {
    case(Pokemon::xpCurve::Erratic): {
        expReq = erratic(target) - initExp;
        break;
    }
    case(Pokemon::xpCurve::Fast): {
        expReq = fast(target) - initExp;
        break;
    }
    case(Pokemon::xpCurve::MediumSlow): {
        expReq = mediumSlow(target) - initExp;
        break;
    }
    case(Pokemon::xpCurve::MediumFast): {
        expReq = mediumFast(target) - initExp;
        break;
    }
    case(Pokemon::xpCurve::Slow): {
        expReq = slow(target) - initExp;
        break;
    }
    case(Pokemon::xpCurve::Fluctuating): {
        expReq = fluctuating(target) - initExp;
        break;
    }
    }

    std::cout << "Pokemon needs " << expReq << " exp." << std::endl;
    int XL = 0, L = 0, M = 0, S = 0, XS = 0;
    while(expReq > 0) {
        if(expReq >= 30000) {
            ++XL;
            expReq -= 30000;
        } else if(expReq >= 10000) {
            ++L;
            expReq -= 10000;
        } else if(expReq >= 3000) {
            ++M;
            expReq -= 3000;
        } else if(expReq >= 800) {
            ++S;
            expReq -= 800;
        } else {
            ++XS;
            expReq -= 100;
        }
    }
    std::cout << "or "
              << " XL: " << XL << " L: " << L << " M: " << M << " S: " << S << " XS: " << XS << " candies" << std::endl;
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

    std::cout << "XP Curve [Erratic = 0, Fast = 1, MediumFast = 2, MediumSlow = 3, Slow = 4, Fluctuating = 5]"
              << std::endl;
    int levelVal;
    std::cin >> levelVal;
    curve = Pokemon::xpCurve(levelVal);

    std::cout << "When does/how it level up [int/string]" << std::endl;
    std::cin.ignore(256, '\n');
    std::getline(std::cin, levelUp);

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