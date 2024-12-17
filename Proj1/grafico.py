import numpy as np
import matplotlib.pyplot as plt

def perform_separate_fits(data_file, data_file_without_heuristic):
    # Função para ler dados de um ficheiro e processá-los
    def read_data(file_path):
        data = []
        with open(file_path, 'r') as file:
            for line in file:
                cols = line.strip().split()
                # Remover "ms" da terceira coluna e converter para float
                cols[1] = float(cols[1].replace("ms", ""))
                data.append([float(cols[0]), cols[1]])
        return np.array(data)

    # Ler os dados dos dois ficheiros
    data = read_data(data_file)
    data_without_heuristic = read_data(data_file_without_heuristic)

    # Separar as colunas (primeiro conjunto de dados)
    x = data[:, 0]  # Eixo X
    y = data[:, 1]  # Eixo Y

    # Separar as colunas (segundo conjunto de dados)
    x_no_heuristic = data_without_heuristic[:, 0]
    y_no_heuristic = data_without_heuristic[:, 1]

    # Fit de uma regressão polinomial de grau 3 (com heurística)
    degree = 3
    coefficients = np.polyfit(x, y, degree)
    polynomial = np.poly1d(coefficients)
    x_fit = np.linspace(min(x), max(x), 500)
    y_fit = polynomial(x_fit)

    # Fit de uma regressão polinomial de grau 3 (sem heurística)
    coefficients_no_heuristic = np.polyfit(x_no_heuristic, y_no_heuristic, degree)
    polynomial_no_heuristic = np.poly1d(coefficients_no_heuristic)
    x_fit_no_heuristic = np.linspace(min(x_no_heuristic), max(x_no_heuristic), 500)
    y_fit_no_heuristic = polynomial_no_heuristic(x_fit_no_heuristic)

    # Valor de referência para a linha vertical
    x_limit = 3276800000000

    # Primeiro gráfico: Dados com heurística
    plt.figure(figsize=(8, 6))
    plt.scatter(x, y, color="blue", label="Dados (com heurística)")
    plt.plot(x_fit, y_fit, color="red", label="Ajuste Polinomial")
    plt.axvline(x=x_limit, color="black", linestyle="--", label="Último teste sem heurística")
    plt.xlabel("Complexidade (O(m³⋅n²))")
    plt.ylabel("Tempo de Execução (ms)")
    plt.title("Análise Assintótica (com heurística)")
    plt.legend(loc="lower right")
    plt.grid(True)
    plt.show()

    # Segundo gráfico: Dados sem heurística
    plt.figure(figsize=(8, 6))
    plt.scatter(x_no_heuristic, y_no_heuristic, color="green", label="Dados (sem heurística)")
    plt.plot(x_fit_no_heuristic, y_fit_no_heuristic, color="orange", label="Ajuste Polinomial")
    plt.xlabel("Complexidade (O(m³⋅n²))")
    plt.ylabel("Tempo de Execução (ms)")
    plt.title("Análise Assintótica (sem heurística)")
    plt.legend(loc="lower right")
    plt.grid(True)
    plt.show()

# Chamada da função com dois ficheiros (substitua pelos caminhos corretos)
perform_separate_fits("dados.txt", "dados_sem.txt")
