#include "Jinja2Tools/PluginContext.h"
#include "Jinja2Tools/PluginCommands.h"
extern HMODULE g_module;
extern "C" __declspec(dllexport) void setInfo(NppData d){jinja2tools::PluginContext::instance().initialize(g_module,d);jinja2tools::initializeCommands();}
extern "C" __declspec(dllexport) const wchar_t* getName(){return L"Jinja2 Tools";}
extern "C" __declspec(dllexport) FuncItem* getFuncsArray(int*c){return jinja2tools::getCommandItems(c);}
extern "C" __declspec(dllexport) void beNotified(SCNotification*){}
extern "C" __declspec(dllexport) LRESULT messageProc(UINT,WPARAM,LPARAM){return TRUE;}
extern "C" __declspec(dllexport) BOOL isUnicode(){return TRUE;}
