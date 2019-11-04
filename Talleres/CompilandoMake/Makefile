cc= gcc
flag= -Wall
header= validar.h guardar.h

obj= validar.o guardar.o main.o

bin= usuarios

$(bin) : $(obj)
	@$(cc) $(flag) -o $@ $?

%.o : %.c $(header)
	@$(cc) $(flag) -c $<

go : $(bin)
	@./$<
	
.PHONY : clean
clean : 
	@rm *.o
	@rm $(bin)