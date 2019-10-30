#include <stdio.h>
#include "validar.h"
#include "guardar.h"

/*
Nombre 
Apellido
username (generado)
password 
userid  (generado)
*/

int main()
{

    Usuario_t u;

    printf("Ingrese Nombre: ");
    scanf("%s", u.nombre);

    printf("\nIngrese Apellido: ");
    scanf("%s", u.apellido);

    printf("\nIngrese Contrasenia: ");
    scanf("%s", u.password);

    printf("\n------------------------------------------------\n");

    int val = validar(&u);

    switch (val)
    {
    case 3:
        printf("Password no size\n");
        break;
    case 4:
        printf("Letra no in password\n");
        break;
    case 5:
        printf("Number no in password\n");
        break;
    case 0:
        printf("\nCorrecto\n");
        break;
    default:
        printf("Ninguna de las Anteriores");
        break;
    }

    return val;
}
