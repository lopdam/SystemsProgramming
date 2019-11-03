#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <validar.h>

int validar(Usuario_t *u)
{
  u->nombre[0] = toupper(u->nombre[0]);
  u->apellido[0] = toupper(u->apellido[0]);
  //id
  srand(time(NULL));
  u->userid = rand();

  //minuscula Nombre
  for (int i = 1; i <= strlen(u->nombre); i++)
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
      number = 0;
    }
    if ((u->password[i] >= 65 && u->password[i] <= 90) || (u->password[i] >= 97 && u->password[i] <= 122))
    {
      letras = 0;
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
  return 0;
}