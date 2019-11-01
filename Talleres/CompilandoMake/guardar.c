#include <stdio.h>
#include "validar.h"
#include "guardar.h"


/* Usuario_t * es el puntero al arreglo de usuarios */
void guardar(Usuario_t *users){
   FILE* puntero;
   puntero = fopen("usuario.txt","w+");
      
      fprintf(puntero,"%s  %s  %s  %s  %d",users->nombre,users->apellido,users->username,users->password,users->userid);
 
      fclose(puntero);
      return;

}