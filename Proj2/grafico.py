import numpy as np
import matplotlib.pyplot as plt

def perform_fit(data_file):
    # Função para ler dados de um ficheiro e processá-los
    def read_data(file_path):
        data = []
        with open(file_path, 'r') as file:
            for line in file:
                cols = line.strip().split()
                # Remover "ms" da segunda coluna e converter para float
                cols[1] = float(cols[1].replace("ms", ""))
                data.append([float(cols[0]), cols[1]])
        return np.array(data)

    # Ler os dados do ficheiro
    data = read_data(data_file)

    # Separar as colunas (dados)
    x = data[:, 0]  # Eixo X
    y = data[:, 1]  # Eixo Y

    # Fit de uma regressão polinomial de grau 3
    degree = 3
    coefficients = np.polyfit(x, y, degree)
    polynomial = np.poly1d(coefficients)
    x_fit = np.linspace(min(x), max(x), 500)
    y_fit = polynomial(x_fit)

    # Gráfico: Dados com ajuste
    plt.figure(figsize=(8, 6))
    plt.scatter(x, y, color="blue", label="Dados experimentais")
    plt.plot(x_fit, y_fit, color="red", label="Ajuste Polinomial")
    plt.xlabel("Complexidade (O(max(m·l, l²·n, l³))")
    plt.ylabel("Tempo de Execução (ms)")
    plt.title("Análise Assintótica para o Índice de Conectividade")
    plt.legend(loc="lower right")
    plt.grid(True)
    plt.show()

# Chamada da função com o ficheiro correto (substitua pelo caminho correto)
perform_fit("dados.txt")
