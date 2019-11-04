#include <stdio.h>
#include <stdlib.h>
#include "validar.h"
#include "guardar.h"

void mensaje();
int main()
{
    Usuario_t us[MaxUsers];
    char opcion[1];
    int numUsuarios = 0;

    do
    {
        mensaje();
        scanf("%s",opcion);
  
        if ((numUsuarios + 1) >= MaxUsers)
        {
            fprintf(stderr,"\n**Max Users Alcanzado**\n");
            opcion[0]= '2';
        }

        else if (opcion[0] == '1')
        {
            Usuario_t u;
            int val;
            printf("\n------------------------------------------------\n");
            printf("Ingrese Nombre: ");
            scanf("%s", u.nombre);

            printf("\nIngrese Apellido: ");
            scanf("%s", u.apellido);

            printf("\nIngrese Contrasenia: ");
            scanf("%s", u.password);
            val = validar(&u);

            /* Detectar error */
            switch (val)
            {
            case 3:
                fprintf(stderr, "\n**Password no size**\n");
                break;
            case 4:
                fprintf(stderr, "\n**Letra no in password**\n");
                break;
            case 5:
                fprintf(stderr, "\n**Number no in password**\n");
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

    } while (opcion[0] != '2');
    printf("Exit\n");
    guardar(us, numUsuarios);

    return 0;
}

void mensaje()
{
    printf("\n------------------------------------------------\n");
    printf("\n   1. Desea agregar un usuario:");
    printf("\n   2. Salir.");
    printf("\n\n   Introduzca opcion (1-2): ");
}
