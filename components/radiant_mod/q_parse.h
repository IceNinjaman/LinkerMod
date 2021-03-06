#pragma once

void Com_BeginParseSession(const char *filename);
void Com_EndParseSession();
void Com_SetSpaceDelimited(int spaceDelimited);
void Com_SetKeepStringQuotes(int keepStringQuotes);
void Com_SetScriptWarningPrefix(const char *prefix);
void Com_UngetToken();
void Com_ScriptError(const char *msg, ...);
const char *Com_Parse(const char **data_p);
int Com_MatchToken(const char **buf_p, const char *match, int warning);
void Com_SkipRestOfLine(const char **data);
float Com_ParseFloat(const char **buf_p);
int Com_ParseInt(const char **buf_p);