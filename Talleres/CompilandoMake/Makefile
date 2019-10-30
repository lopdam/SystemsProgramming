cc= gcc
flag= -Wall

source= main.c validar.c guardar.c

obj= main.o validar.o guardar.o 

bin= CompilandoMake.out

$(bin) : $(obj)
	$(cc) $(flag) -o $@ $?

%.o : %.c 
	$(cc) $(flag) -c $<

x : $(bin)
	./$<
.PHONY : clean
clean : 
	rm *.o 
	rm $(bin)