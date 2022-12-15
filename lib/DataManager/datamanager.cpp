/**
 * @file datamanager.cpp
 * @author Hayden Lauritzen (haydenlauritzen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <fstream>
#include <iostream>
#include <map>
#include <string>

/**
 * @brief
 * name    - Name of Pokemon
 * dexNum  - NatDex number
 * xpCurve - xpCurve type --> determines xp : lvl ratio
 * evolve - level which Pokemon evolves
 */
struct Pokemon {
    enum xpCurve { Erratic, Fast, MediumFast, MediumSlow, Slow, Fluctuating };

    std::string name;
    int dexNum;
    xpCurve curve;
    std::string evolve;

    Pokemon() {}
    Pokemon(int dexNum, std::string name, xpCurve curve, std::string evolve)
        : name(name), dexNum(dexNum), curve(curve), evolve(evolve) {}

    bool operator==(const Pokemon& rhs) {
        return this->name == rhs.name;
    }
    friend std::ostream& operator<<(std::ostream& os, const Pokemon& p) {
        os << p.dexNum << ' ' << p.name << ' ' << p.curve << ' ' << p.evolve;
        return os;
    }
};

class PokemonManager {

public:
    /**
     * @brief Writes map contents to file
     * @tparam T - map value
     * @param data - map of data to write
     * @param id   - filename label
     */
    void write(const std::map<std::string, Pokemon>& data, std::string id) {
        std::string filename = "data/" + id + ".dat";
        // std::string filename = id + ".dat";
        std::ifstream ifs(filename);
        // does not override values in current file
        auto curData = read(filename);
        curData.insert(data.begin(), data.end());
        ifs.close();
        // clear file
        std::ofstream ofs(filename, std::fstream::trunc);
        // write file
        for(const auto& [key, value] : data) {
            std::string line;
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            ofs << value << '\n';
        }
        ofs.close();
    }

    /**
     * @brief Reads map contents from file
     * To be implemented per data type
     * @tparam T - map value
     * @param id   - filename label
     * @return std::map<int, T> data from file
     */
    std::map<std::string, Pokemon> read(std::string id) {

        std::map<std::string, Pokemon> dex;

        std::string filename = "data/" + id + ".dat";
        std::ifstream fs;
        fs.open(filename);

        int dexNum, curve;
        std::string name, evolve;
        while(fs >> dexNum >> name >> curve) {
            std::getline(fs, evolve);
            Pokemon p(dexNum, name, Pokemon::xpCurve(curve), evolve);
            dex.insert(std::make_pair(name, p));
        }
        return dex;
    }
};
