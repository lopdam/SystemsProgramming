entradas=main.c validar.c
salida= almacernarDatos.out

all: $(salida)
	@./$(salida)
	@rm -f $(salida)

$(salida): $(entradas)
	@gcc $(entradas) -o $(salida) -Wall
clear :
	@rm -f $(salida)