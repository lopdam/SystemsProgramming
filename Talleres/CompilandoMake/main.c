#include <stdio.h>
#include <stdlib.h>
#include "validar.h"
#include "guardar.h"

int main()
{
    Usuario_t us[MaxUsers];
    char opcion;
    int numUsuarios = 0;

    do
    {
        int val;

        Usuario_t u;
        printf("\n------------------------------------------------\n");
        printf("\n   1. Desea agregar un usuario:");
        printf("\n   2. Salir.");
        printf("\n\n   Introduzca opcion (1-2): ");
        scanf("%c", &opcion);

        if ((numUsuarios + 1) >= MaxUsers)
        {
            printf("Max User Alcanzado\n");
            opcion = '2';
        }

        if (opcion == '1')
        {
            printf("\n------------------------------------------------\n");
            printf("Ingrese Nombre: ");
            scanf("%s", u.nombre);

            printf("\nIngrese Apellido: ");
            scanf("%s", u.apellido);

            printf("\nIngrese Contrasenia: ");
            scanf("%s", u.password);
            val = validar(&u);
            /* Fin del anidamiento */
            switch (val)
            {
            case 3:
                fprintf(stderr, "**Password no size**\n");
                break;
            case 4:
                fprintf(stderr, "**Letra no in password**\n");
                break;
            case 5:
                fprintf(stderr, "**Number no in password**\n");
                break;
            case 0:
                fprintf(stderr, "\n**Correcto**\n");
                us[numUsuarios] = u;
                ++numUsuarios;
                break;
            default:
                fprintf(stderr, "Ninguna de las Anteriores");
                break;
            }
        }
    } while (opcion != '2');
    printf("Exit\n");
    guardar(us, numUsuarios);

    return 0;
}
