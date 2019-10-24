#include "validar.h"
#include<ctype.h>

int validar(Usuario_t *u)
{
  u->nombre[0]=toupper(u->nombre[0]);
  u->apellido[0]=toupper(u->apellido[0]);

  u->username[0]=tolower(u->nombre[0]);

  for(int i=0;i<sizeof(u->apellido);i++){

       u->username[i+1]=tolower(u->apellido[i]);
  }

    return 0;
};