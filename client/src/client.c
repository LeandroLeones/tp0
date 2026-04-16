#include "client.h"


int main(void)
{

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

    logger = iniciar_logger();

    config = iniciar_config();

     ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    valor = config_get_string_value(config, "CLAVE");

    log_info(logger, "Valor de CLAVE: %s", valor);

    conexion = crear_conexion(ip, puerto);

    enviar_mensaje(valor, conexion);

    paquete(conexion);

    terminar_programa(conexion, logger, config);

    return 0;
}

t_log* iniciar_logger(void)
{
    t_log* nuevo_logger = log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);

    if (nuevo_logger == NULL) {
        printf("No se pudo crear el logger\n");
        exit(1);
    }

    return nuevo_logger;
}

t_config* iniciar_config(void)
{
    t_config* nuevo_config = config_create("cliente.config");

    if (nuevo_config == NULL) {
        printf("No se pudo leer el archivo de configuración\n");
        exit(1);
    }

    return nuevo_config;
}


void leer_consola(t_log* logger)
{
    char* leido = readline("> ");
    log_info(logger, "Ingresaste: %s", leido);

    while (strcmp(leido, "") != 0) {
        free(leido);                     
        leido = readline("> ");          
        log_info(logger, "Ingresaste: %s", leido);
    }

    free(leido);
}

void paquete(int conexion)
{
    char* leido;
    t_paquete* paquete = crear_paquete();

    while ((leido = readline("> ")) && strcmp(leido, "") != 0) {
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
    }

    free(leido);

    enviar_paquete(paquete, conexion);

    eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
    liberar_conexion(conexion);

    if (logger != NULL) {
        log_destroy(logger);
    }

    if (config != NULL) {
        config_destroy(config);
    }
}

