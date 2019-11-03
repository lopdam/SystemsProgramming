cc= gcc
flag= -Wall
header= guardar.h validar.h
source= main.c validar.c guardar.c $(header)

obj= main.o guardar.o validar.o 

bin= CompilandoMake.out

$(bin) : $(obj)
	@$(cc) $(flag) -o $@ $?

%.o : %.c $(header)
	@$(cc) $(flag) -c $< -I.

go : $(bin)
	@./$<
	
.PHONY : clean
clean : 
	@rm *.o
	@rm usuario.txt
	@rm $(bin)