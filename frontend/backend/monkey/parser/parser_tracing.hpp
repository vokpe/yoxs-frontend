#ifndef PARSER_TRACING_H
#define PARSER_TRACING_H

#include <string>

std::string identLevel();

void tracePrint(const std::string &msg);
void incIdent();
void decIdent();
std::string trace(const std::string &msg);
void untrace(const std::string &msg);

#endif // PARSER_TRACING_H
