#include "validar.h"
#include<ctype.h>
#include<string.h>
#include<stdio.h>

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
  u->nombre[0]=toupper(u->nombre[0]);
  u->apellido[0]=toupper(u->apellido[0]);

  u->username[0]=tolower(u->nombre[0]);

  for(int i=0;i<=strlen(u->apellido);i++){
       u->username[i+1]=tolower(u->apellido[i]);
  }

  printf("\nName: %s\n",u->nombre);
  printf("LastName: %s\n",u->apellido);
  printf("UserName: %s\n",u->username);
  printf("PassWord: %s\n",u->password);
  printf("IdUser: %d\n",u->userid);

    return 0;
};