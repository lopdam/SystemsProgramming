#include "validar.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Validar
password debe de tener mínimo 1 letra, mínimo un digito y mínimo 10 caracteres en total

La función validar() debe de retornar:
0 si no hubo ningún error
3 si password < 10
4 si password no contiene letras
5 si password no contiene dígitos

*/
int validar(Usuario_t *u)
{
  u->nombre[0] = toupper(u->nombre[0]);
  u->apellido[0] = toupper(u->apellido[0]);
  //id
  u->userid = rand();

  //minuscula Nombre
  for (int i = 0; i <= strlen(u->nombre); i++)
  {
    u->nombre[i] = tolower(u->nombre[i]);
  }
  //Minuscula apellido
  for (int i = 1; i < strlen(u->apellido); i++)
  {
    u->apellido[i] = tolower(u->apellido[i]);
  }

  u->username[0] = tolower(u->nombre[0]);
  //Agregar apellido al usename
  for (int i = 0; i <= strlen(u->apellido); i++)
  {
    u->username[i + 1] = tolower(u->apellido[i]);
  }

  if (strlen(u->password) < 10)
  {
    return 3;
  }

  int letras = 1;
  int number = 1;

  for (int i = 0; i < strlen(u->password); i++)
  {
    if (u->password[i] >= 48 && u->password[i] <= 57)
    {
      letras = 0;
    }
    if ((u->password[i] >= 65 && u->password[i] <= 90) || (u->password[i] >= 97 && u->password[i] <= 122))
    {
      number = 0;
    }
  }

  if (letras)
  {
    return 4;
  }
  if (number)
  {
    return 5;
  }

  printf("\nName: %s\n", u->nombre);
  printf("LastName: %s\n", u->apellido);
  printf("UserName: %s\n", u->username);
  printf("PassWord: %s\n", u->password);
  printf("IdUser: %d\n", u->userid);

  return 0;
};