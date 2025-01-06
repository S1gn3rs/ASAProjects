#!/bin/python3
from pulp import *

import time

start_timeT = time.time()

# Entrada
n, m, t = map(int, input().split())
factories = []
dictFactories = {}
kidsByCountry = {}
kidsByFactory = {}
kids = {}
countries = []
wishesExport = {}
factoriesWithNoStock = set()


for _ in range (n): # idFactory idCountry maxStock ## TALVEZ MUDAR PARA DICIONARIO
    idFact, idCountry, maxStock = map(int, input().split())
    if maxStock == 0:
        factoriesWithNoStock.add(idFact)
        continue
    if maxStock < 0:
        print(-1)
        exit(0)
    factories.append((idFact, idCountry, maxStock))
    kidsByFactory[idFact] = []
    dictFactories[idFact] = {"country": idCountry, "maxStock": maxStock}


for _ in range(m): # idCountry pMax pMin
    countries.append(tuple(map(int, input().split())))
    kidsByCountry[countries[-1][0]] = []
    wishesExport[countries[-1][0]] = []


i = 0
for _ in range(t): # idKid idCountry idFactories
    data = list(map(int, input().split()))
    idKid, idCountry, *toys = data  # Separa os dois primeiros e o restante

    if toys == set(): continue

    factoriesSet = set(toys)  # Converte os IDs das fábricas em um conjunto
    for idFact in toys:
        # if (idFact in dictFactories):
        #     x[(idKid, idFact)] = LpVariable(f"x_{idKid}_{idFact}", 0, 1, cat='Binary')
        if (idFact in factoriesWithNoStock):
            factoriesSet.remove(idFact)
            continue

        kidsByFactory[idFact].append(idKid)
        factCountry = dictFactories[idFact]["country"]
        if idCountry != factCountry:
            wishesExport[factCountry].append((idKid, idFact))

    kids[idKid] = {"country": idCountry, "toys": factoriesSet}

    kidsByCountry[idCountry].append(idKid)


# ----------------------------------------------------------------------------------------------------------


# Criar o modelo
modelo = LpProblem("Distribuicao_de_Brinquedos", LpMaximize)

# ----------------------------------------------------------------------------------------------------------


# # Variáveis de decisão (apenas fábricas requisitadas pela criança)
x = LpVariable.dicts("x", [(idKid, idFact) for idKid in kids.keys() for idFact in kids[idKid]["toys"]], cat='Binary')

# ----------------------------------------------------------------------------------------------------------


# Função objetivo: Maximizar o número de crianças atendidas
modelo += lpSum(x[idKid, idFact] for idKid in kids.keys() for idFact in kids[idKid]["toys"]), "Objetivo"



# Restrições
# Cada criança recebe no máximo um presentefor idKid in kids.keys():    modelo += lpSum(x[idKid, idFact] for idFact in kids[idKid]["toys"]) <= 1
# Aggregate valid pairs of (idKid, idFact) for each child
kid_constraints = { idKid: lpSum(x[idKid, idFact] for idFact in kids[idKid]["toys"]) for idKid in kids}

# Add constraints using the precomputed sums
for idKid, sum_expr in kid_constraints.items():
    modelo += sum_expr <= 1

# Estoque máximo de cada fábrica
for idFact, _, fmax in factories:
    modelo += lpSum(x[idKid, idFact] for idKid in kidsByFactory[idFact]) <= fmax


# Restrições de exportação e mínimo por país
for idCountry, pmax, pmin in countries:
    # Limite de exportação do país

    modelo += lpSum(x[idKidFact[0], idKidFact[1]] for idKidFact in wishesExport[idCountry]) <= pmax

    # Número mínimo de presentes no país
    if pmin == 0: continue
    modelo += lpSum(x[idKid, idFact] for idKid in kidsByCountry[idCountry] for idFact in kids[idKid]["toys"] ) >= pmin


# ----------------------------------------------------------------------------------------------------------
# Resolver o modelo
status = modelo.solve(GLPK(msg=0))

# ----------------------------------------------------------------------------------------------------------
# Output
if status == LpStatusOptimal:
    print(int(value(modelo.objective)))
else:
    print(-1)


end_time = time.time()


execution_time = end_time - start_timeT
print(f"Tempo de execução Total: {execution_time:.4f} segundos")
