source= main.c validar.c guardar.c
obj= main.o validar.o guardar.o 

bin= CompilandoMake.out

all : $(bin)
	./$<

$(bin) : $(obj)
	gcc -Wall -o $@ $?

%.o : %.c 
	gcc -Wall -c $<

.PHONY : clean

clean : 
	rm *.o 
	rm $(bin)