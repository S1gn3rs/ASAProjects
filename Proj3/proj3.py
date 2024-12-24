#!/bin/python3
from pulp import *

# Entrada
n, m, t = map(int, input().split())
factories = [tuple(map(int, input().split())) for i in range(n)]  # idFabrica idPais stockMaximo ## TALVEZ MUDAR PARA DICIONARIO
countries = [tuple(map(int, input().split())) for i in range(m)]  # idPais pMax pMin
kids = [tuple(map(int, input().split())) for i in range(t)]  # idCrianca idPais idFactories



# Dados das fábricas (idFabrica -> {idPais, stockMaximo})
dictFactories = {factory[0]: {"country": factory[1], "stock": factory[2]} for factory in factories}


# EM PRINCIPIO NAO PRECISO DISTO
# # Dados dos países (idPais -> {pMax, pMin})
# countries = {}
# for _ in range(m):
#     idCountry, pMax, pMin = map(int, input().split())
#     countries[idCountry] = {"pMax": pMax, "pMin": pMin}

# # Dados das crianças (idCrianca -> {idPais, idFactories})
# kids = {}
# for _ in range(t):
#     data = list(map(int, input().split()))
#     idKid, idCountry = data[0], data[1]
#     factoriesRequested = data[2:]
#     kids[idKid] = {"country": idCountry, "factories": factoriesRequested}



# ----------------------------------------------------------------------------------------------------------


# Criar o modelo
modelo = LpProblem("Distribuicao_de_Brinquedos", LpMaximize)

# ----------------------------------------------------------------------------------------------------------


# Variáveis de decisão (apenas fábricas requisitadas pela criança)
x = LpVariable.dicts("x", ((kid[0], idFact) for kid in kids for idFact in kid[2:]), cat='Binary')

# ----------------------------------------------------------------------------------------------------------


# Função objetivo: Maximizar o número de crianças atendidas
modelo += lpSum(x[kid[0], idFact] for kid in kids for idFact in kid[2:]), "Objetivo"

# ----------------------------------------------------------------------------------------------------------


# Restrições
# Cada criança recebe no máximo um presente
for kid in kids:
    modelo += lpSum(x[kid[0], idFact] for idFact in kid[2:]) <= 1

# Estoque máximo de cada fábrica
for idFact, idCountry, fmax in factories:
    modelo += lpSum(x[kid[0], idFact] for kid in kids if idFact in kid[2:]) <= fmax

# Restrições de exportação e mínimo por país
for idCoun, pmax, pmin in countries:
    # Limite de exportação do país
    modelo += lpSum(x[kid[0], idFact] for kid in kids for idFact in kid[2:]
                     if dictFactories[idFact]["country"] == idCoun and kid[1] != idCoun) <= pmax
    # Número mínimo de presentes no país
    modelo += lpSum(x[kid[0], idFact] for kid in kids for idFact in kid[2:]
                     if kid[1] == idCoun) >= pmin

# ----------------------------------------------------------------------------------------------------------
# Resolver o modelo
status = modelo.solve()

# ----------------------------------------------------------------------------------------------------------
# Output
if status == LpStatusOptimal:
    print(int(value(modelo.objective)))
else:
    print(-1)
