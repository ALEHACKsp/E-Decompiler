#pragma once
#include <ida.hpp>

struct outputFunc
{
	ea_t FuncAddr;
	qstring FuncName;
};

struct EsigInfo
{
	qstring Category;
	qstring Name;
	qstring Description;
	qstring Path;
};

//ѡ���������ļ�
qstring ChooseEsig();

//Ӧ���������ļ�
bool ApplyEsig(qstring sigPath);