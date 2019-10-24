
#define MAXSTR 50

int validar(Usuario_t *u);

typedef struct Usuario
{
    char nombre[MAXSTR];
    char apellido[MAXSTR];
    char username[MAXSTR];
    char password[MAXSTR];
    int userid;
} Usuario_t;
