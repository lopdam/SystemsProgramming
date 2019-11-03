#include <stdio.h>
#include "validar.h"
#include "guardar.h"

void guardar(Usuario_t *users, int n)
{
      FILE *puntero;
      puntero = fopen("usuario.txt", "w+");

      for (int i = 0; i < n; i++)
      {
            fprintf(puntero, "%s  %s  %s  %s  %d\n", users[i].nombre, users[i].apellido, users[i].username, users[i].password, users[i].userid);
      }

      fclose(puntero);
}