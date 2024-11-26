# Nome do executável
EXEC = exec

# Nome do arquivo fonte
SRC = Final.cpp

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -pedantic -Werror -std=c++20 \
           -O2 -fsanitize=address,undefined -fstack-protector-strong

# Regras padrão
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(EXEC)

# Regra para verificar warnings com apenas análise estática (sem gerar binário)
check:
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SRC)

# Regra para rodar o programa
run: $(EXEC)
	./$(EXEC)

# Garante que o Makefile não use arquivos com os mesmos nomes como dependências
.PHONY: all clean check run
