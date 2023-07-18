import pandas as pd
import numpy as np
import math

def getRow(nat_id):
    df = pd.read_csv('data/xp.dat')
    try:
      return df.iloc[nat_id]
    except IndexError:
      return None

def getNatDex(name):
  df = pd.read_csv('data/xp.dat')
  try:
    return df.loc[name, 'Ndex']
  except KeyError:
    return None

def getName(nat_id):
    row = getRow(nat_id)
    if row is None:
        return None
    return row['Pokémon']

def getExpType(nat_id):
    row = getRow(nat_id)
    if row is None:
        return None
    return row['Experience Type']

def getLvlUp(nat_id):
    row = getRow(nat_id)
    if row is None:
        return None
    level = row['Level Up']
    if np.isnan(level):
        return 'Does not level up with experience'
    return row['Level Up']

def calcErratic(level):
    if level < 50:
        return level**3 * (100 - level) / 50
    elif level < 68:
        return level**3 * (150 - level) / 100
    elif level < 98:
        return level**3 * ((1911 - 10 * level) / 3) / 500
    else:
        return level**3 * (160 - level) / 100
    
def calcFast(level):
    return 4 * level**3 / 5

def calcMedFast(level):
    return level**3

def calcMedSlow(level):
    return 6 / 5 * level**3 - 15 * level**2 + 100 * level - 140

def calcSlow(level):
    return 5 * level**3 / 4

def calcFluct(level):
    if level < 15:
        return level**3 * ((level + 1) / 3 + 24) / 50
    elif level < 36:
        return level**3 * (level + 14) / 50
    else:
        return level**3 * ((level / 2) + 32) / 50

def getReqExp(nat_id, init_exp, calc_level):
    match getExpType(nat_id):
        case 'Erratic':
            return calcErratic(calc_level) - init_exp
        case 'Fast':
            return calcFast(calc_level) - init_exp
        case 'Medium Fast':
            return calcMedFast(calc_level) - init_exp
        case 'Medium Slow':
            return calcMedSlow(calc_level) - init_exp
        case 'Slow':
            return calcSlow(calc_level) - init_exp
        case 'Fluctuating':
            return calcFluct(calc_level) - init_exp
        
def getCurLevel(nat_id, cur_exp):
    exp_needed = 0
    exp_type = getExpType(nat_id)
    
    if exp_type == 'Erratic':
        for level in range(1, 101):
            exp_needed = calcErratic(level)
            if cur_exp < exp_needed:
                return level - 1

    elif exp_type == 'Fast':
        for level in range(1, 101):
            exp_needed = calcFast(level)
            if cur_exp < exp_needed:
                return level - 1

    elif exp_type == 'Medium Fast':
        for level in range(1, 101):
            exp_needed = calcMedFast(level)
            if cur_exp < exp_needed:
                return level - 1

    elif exp_type == 'Medium Slow':
        for level in range(1, 101):
            exp_needed = calcMedSlow(level)
            if cur_exp < exp_needed:
                return level - 1

    elif exp_type == 'Slow':
        for level in range(1, 101):
            exp_needed = calcSlow(level)
            if cur_exp < exp_needed:
                return level - 1

    elif exp_type == 'Fluctuating':
        for level in range(1, 101):
            exp_needed = calcFluct(level)
            if cur_exp < exp_needed:
                return level - 1
    
    return 100

def run():
    EXIT = 'x'
    EVOLVE = 'e'
    MIN_DEX = 1
    MAX_DEX = 1010
    MIN_LEVEL = 1
    MAX_LEVEL = 100
    NOT_SET = -1

    inp = ''
    nat_id = NOT_SET
    init_exp = NOT_SET
    calc_level = NOT_SET
    exp_curve = NOT_SET

    while inp != 'x':
        while nat_id == NOT_SET:
          inp = input('Enter a national dex number, Pokémon name, or "x" to exit: ')
          if inp == EXIT:
              return False
          try:
              inp = int(inp)
              nat_id = inp
          except ValueError:
              nat_id = getNatDex(inp)
          if inp < MIN_DEX or inp > MAX_DEX:
              print('Invalid national dex number')
              nat_id = NOT_SET
              
        while init_exp == NOT_SET:
          inp = input('Current Pokémon Exp: ')
          try:
              inp = int(inp)
              if inp < 0 or inp > getReqExp(nat_id, 0, MAX_LEVEL):
                  print('Invalid exp amount')
                  init_exp = NOT_SET
              elif inp == getReqExp(nat_id, 0, MAX_LEVEL):
                  print('Pokémon is max level')
                  return True
              init_exp = inp             
          except ValueError:
              print('Invalid input')
              init_exp = NOT_SET     

        while calc_level == NOT_SET:
          inp = input('Enter a level or "e" to evolve: ')
          if inp == EVOLVE:
              calc_level = getLvlUp(nat_id)  
              if calc_level == 'Does not level up with experience':
                  calc_level = NOT_SET
              elif init_exp > getReqExp(nat_id, 0, calc_level):
                  print('Pokémon is past evolve level')
                  calc_level = min(getCurLevel(nat_id, init_exp) + 1, MAX_LEVEL)
          elif inp >= MIN_LEVEL and inp <= MAX_LEVEL:
              calc_level = inp

        req_exp = getReqExp(nat_id, init_exp, calc_level)

        print(f'{getName(nat_id)} needs {req_exp} experience to reach level {calc_level}')
        XL = req_exp // 30000
        req = req_exp % 30000
        L = req // 10000
        req = req % 10000
        M = req // 3000
        req = req % 3000
        S = req // 800
        req = req % 800
        XS = math.ceil(req / 100) 

        print(f'{XL} XL, {L} L, {M} M, {S} S, {XS} XS')

        inp = ''
        nat_id = NOT_SET
        init_exp = NOT_SET
        calc_level = NOT_SET
        exp_curve = NOT_SET

def main():
    while run():
        pass
        

if __name__ == '__main__':
    main()