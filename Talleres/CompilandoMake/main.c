#include <stdio.h>
#include <stdlib.h>
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
    Usuario_t us[MaxUsers];
    int opcion;
    int numUsuarios=0;

    do
    {
        int val;

        Usuario_t u;
        printf("\n   1. Desea agregar un usuario:");
        printf("\n   2. Salir.");
        printf("\n\n   Introduzca opcion (1-2): ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            printf("Ingrese Nombre: ");
            scanf("%s", u.nombre);

            printf("\nIngrese Apellido: ");
            scanf("%s", u.apellido);

            printf("\nIngrese Contrasenia: ");
            scanf("%s", u.password);

            printf("\n------------------------------------------------\n");
            val = validar(&u);

            break;

        case 2:
            //exit(-1);
            break;
        }

        /* Fin del anidamiento */
        switch (val)
        {
        case 3:
            fprintf(stderr, "Password no size\n");
            break;
        case 4:
            fprintf(stderr, "Letra no in password\n");
            break;
        case 5:
            fprintf(stderr, "Number no in password\n");
            break;
        case 0:
            fprintf(stderr, "\nCorrecto\n");
            us[numUsuarios]=u;
            ++numUsuarios;
            break;
        default:
            fprintf(stderr, "Ninguna de las Anteriores");
            break;
        }

    } while (opcion != 2);
          
      for(int i=0;i<numUsuarios-1;i++){
      printf("%s  %s  %s  %s  %d\n",us[i].nombre,us[i].apellido,us[i].username,us[i].password,us[i].userid);
      //users>>;
} 
    guardar(us,numUsuarios);

    return 0;
}
