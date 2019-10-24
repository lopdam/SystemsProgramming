#include<stdio.h>
#include"validar.h"

/*
Nombre 
Apellido
username (generado)
password 
userid  (generado)
*/

int main(){

    Usuario_t u;

    printf("Ingrese Nombre: ");
    scanf("%s",&u.nombre);

    printf("\nIngrese Apellido: ");
    scanf("%s",&u.apellido);

    printf("\nIngrese Contrasenia: ");
    scanf("%s",&u.password);

    return validar(&u);
}

