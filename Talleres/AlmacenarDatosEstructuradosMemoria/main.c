#include<stdio.h>
#include"validar.h"

/*
Nombre 
Apellido
username (generado)
password (generado)
userid
*/

int main(){

    Usuario_t u;

    printf("Ingrese Nombre: ");
    scanf("%s",&u.nombre);

    printf("\nIngrese Apellido: ");
    scanf("%s",&u.apellido);

    printf("\nIngrese Contrasenia: ");
    scanf("%s",&u.password);

    return 0;
}

