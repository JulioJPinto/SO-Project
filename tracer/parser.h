#ifndef PARSER_H
#define PARSER_H

char **parse_command(char *command_string);

char ***parse_pipeline(char *commands_string);

#endif