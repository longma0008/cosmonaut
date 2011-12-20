#include "log.h"
#include "configuration.h"
#include "cli.h"
#include "networking.h"
#include "../deps/iniparser/iniparser.h"

extern struct GlobalConfig* configuration;

char* get_str_val(dictionary *d, char *prop_name, char *default_value) {
  char *value = iniparser_getstring(d, prop_name, default_value);

  char *config_str_ptr = malloc(strlen(value) + 1);
  strcpy(config_str_ptr, value);

  return config_str_ptr;
}

void load_configuration(int argc, char *argv[]) {
  configuration = malloc(sizeof(GlobalConfig));
  parse_cli_params(argc, argv);

  info("loading config from %s", configuration->config_path);
  dictionary *d = iniparser_load(configuration->config_path);

  configuration->server_port = get_str_val(d, "network:port", "31337");
  configuration->server_name = get_str_val(d, "network:server_name", determine_server_hostname());
  configuration->public_root = get_str_val(d, "app:public_root", ".");
  configuration->socket_queue_size = iniparser_getint(d, "network:socket_queue_size", 50);

  iniparser_freedict(d);
}

void free_configuration() {
  free(configuration->server_port);
  free(configuration->server_name);
  free(configuration->public_root);
  
  free(configuration);
}