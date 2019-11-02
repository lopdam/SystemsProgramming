#include <stdio.h>
#include "validar.h"
#include "guardar.h"


/* Usuario_t * es el puntero al arreglo de usuarios */
void guardar(Usuario_t *users){
<<<<<<< HEAD
    
=======
   FILE* puntero;
   puntero = fopen("usuario.txt","w+");
      
      fprintf(puntero,"%s  %s  %s  %s  %d",users->nombre,users->apellido,users->username,users->password,users->userid);
 
      fclose(puntero);
      return;

>>>>>>> 385949526f47e9df79cc3641488d461d02d017da
}