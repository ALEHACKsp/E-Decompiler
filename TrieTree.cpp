#include "TrieTree.h"
#include "EsigLoader.h"
#include <ida.hpp>
#include <kernwin.hpp>
#include <diskio.hpp>
#include <name.hpp>
#include "public.h"

TrieTreeNode::TrieTreeNode() {
	ChildNodes = new TrieTreeNode * [256];
	for (int i = 0; i < 256; i++) {
		ChildNodes[i] = NULL;
	}
	EsigText = NULL;
	FuncName = NULL;
	IsMatched = false;
}

TrieTree::TrieTree()
{
	root = new TrieTreeNode();
	root->SpecialType = NODE_NORMAL;

	//Ĭ������
	m_IsAligned = false;
	m_IsSetName = true;
	m_MatchSubName = false;
}

TrieTreeNode* TrieTree::AddNode(TrieTreeNode* p, qstring Txt) {
	uchar index = 0;
	HexToBin(Txt, &index);
	if (p->ChildNodes[index]) {
		return p->ChildNodes[index];
	}

	TrieTreeNode* NewNode = new TrieTreeNode(); //������еĽڵ��ж�û��,�򴴽�һ���½ڵ�
	p->ChildNodes[index] = NewNode;      //��ǰ�ڵ�������ӽڵ�

	NewNode->EsigText = new char[Txt.length() + 1]; qstrncpy(NewNode->EsigText, Txt.c_str(), MAXSTR);//��ֵEsigTxt
	MemAllocSave.push_back(NewNode->EsigText);
	NewNode->SpecialType = NODE_NORMAL;
	return NewNode;
}

TrieTreeNode* TrieTree::AddSpecialNode(TrieTreeNode* p, uint type, qstring Txt) {
	for (int i = 0; i < p->SpecialNodes.size(); i++) {		//������ǰ�ӽڵ�
		if (p->SpecialNodes[i]->SpecialType == type && Txt == p->SpecialNodes[i]->EsigText) {
			return p->SpecialNodes[i];
		}
	}
	TrieTreeNode* NewNode = new TrieTreeNode(); //������еĽڵ��ж�û��,�򴴽�һ���½ڵ�
	p->SpecialNodes.push_back(NewNode);      //��ǰ�ڵ�������ӽڵ�
	NewNode->EsigText = new char[Txt.length() + 1]; qstrncpy(NewNode->EsigText, Txt.c_str(), MAXSTR);//��ֵEsigTxt
	MemAllocSave.push_back(NewNode->EsigText);
	NewNode->SpecialType = type;
	return NewNode;
}

bool TrieTree::Insert(qstring& FuncTxt, const qstring& FuncName) {		//����һΪ�������ı���ʽ,������Ϊ����������
	TrieTreeNode* p = root;		//����ǰ�ڵ�ָ��ָ��ROOT�ڵ�

	qstring BasicTxt;
	qstring SpecialTxt;

	MemAllocSave.clear();
	for (UINT n = 0; n < FuncTxt.length(); n++) {
		char test = FuncTxt[n];
		switch (FuncTxt[n])
		{
		case '-':	//Check 1��
			if (FuncTxt[n + 1] == '-' && FuncTxt[n + 2] == '>')
			{
				BasicTxt = "E9";
				p = AddNode(p, BasicTxt);
				p = AddSpecialNode(p, NODE_LONGJMP, "");
				n = n + 2;
				continue;		//��continue�����ⲿѭ��
			}
			return false;
		case '<':
			if (FuncTxt[n + 1] == '[') {						//CALLAPI
				int post = FuncTxt.find("]>", n);
				if (post == -1) {
					return false;
				}
				BasicTxt = "FF";
				p = AddNode(p, BasicTxt);
				BasicTxt = "15";
				p = AddNode(p, BasicTxt);
				SpecialTxt = FuncTxt.substr(n + 2, post);   //�õ��ı��е�IAT����
				p = AddSpecialNode(p, NODE_CALLAPI, SpecialTxt);
				n = post + 1;
				continue;
			}
			else {											//��ͨ�ĺ���CALL
				int post = FuncTxt.find('>', n);
				if (post == -1) {
					return false;
				}
				SpecialTxt = FuncTxt.substr(n + 1, post);
				BasicTxt = "E8";
				p = AddNode(p, BasicTxt);
				p = AddSpecialNode(p, NODE_CALL, SpecialTxt);
				n = post;
				continue;
			}
		case '[':
			if (FuncTxt[n + 1] == ']' && FuncTxt[n + 2] == '>') {
				//To Do
			}
			else
			{
				int post = FuncTxt.find(']', n);
				if (post == -1) {
					return false;
				}
				BasicTxt = "FF";
				p = AddNode(p, BasicTxt);
				BasicTxt = "25";
				p = AddNode(p, BasicTxt);
				SpecialTxt = FuncTxt.substr(n + 1, post);
				p = AddSpecialNode(p, NODE_JMPAPI, SpecialTxt);
				n = post;
				continue;
			}
		case '?':
			if (FuncTxt[n + 1] == '?') {
				p = AddSpecialNode(p, NODE_ALLPASS, FuncTxt.substr(n, n + 2));	//ȫͨ���
				n = n + 1;
				continue;
			}
			else
			{
				p = AddSpecialNode(p, NODE_LEFTPASS, FuncTxt.substr(n, n + 2));	//��ͨ���
				n = n + 1;
				continue;
			}
		case '!':
		{
			int post = FuncTxt.find('!', n + 1);	//��!����һ���ַ���ʼѰ��!
			if (post == -1) {
				return false;
			}
			SpecialTxt = FuncTxt.substr(n + 1, post);
			p = AddSpecialNode(p, NODE_CONSTANT, SpecialTxt);
			n = post;	//����ǰָ��ָ���ұߵ�!��
			continue;
		}
		default:
			if (FuncTxt[n + 1] == '?') {
				p = AddSpecialNode(p, NODE_RIGHTPASS, FuncTxt.substr(n, n + 2));	//��ͨ���
				n = n + 1;
				continue;
			}
			else {
				BasicTxt = FuncTxt.substr(n, n + 2);
				p = AddNode(p, BasicTxt);
				n = n + 1;
				continue;
			}
		}
	}

	if (p->FuncName) {		//ȷ����������Ψһ�ԣ�����
		msg("Find The same Function--%s", p->FuncName);
		for (uint i = 0; i < MemAllocSave.size(); i++)
		{
			delete[] MemAllocSave[i];
		}
		return false;
	}

	p->FuncName = new char[FuncName.length() + 1]; qstrncpy(p->FuncName, FuncName.c_str(), MAXSTR);
	return true;
}

TrieTree::~TrieTree()
{
	TrieTreeNode* p = root;
	if (!p)
	{
		return;
	}

	qstack<TrieTreeNode*> StackNode;	//�ڵ�
	StackNode.push(p);

	while (!StackNode.empty()) {
		p = StackNode.top();
		StackNode.pop();
		//ȡ�ض�ջ���˽ڵ�
		if (p)
		{
			for (uint i = 0; i < 256; i++)
			{
				StackNode.push(p->ChildNodes[i]);
			}
			for (uint j = 0; j < p->SpecialNodes.size(); j++)
			{
				StackNode.push(p->SpecialNodes[j]);
			}
			if (p->EsigText)
			{
				delete[] p->EsigText;
				p->EsigText = NULL;
			}
			if (p->FuncName)
			{
				delete[] p->FuncName;
				p->FuncName = NULL;
			}
			if (p->ChildNodes)
			{
				delete p->ChildNodes;
				p->ChildNodes = NULL;
			}
			p->SpecialNodes.clear();
		}
		delete p;
		p = NULL;
	}
}

bool TrieTree::LoadSig(const char* lpMapPath)
{
	FILE* hFile = fopenRB(lpMapPath);
	if (!hFile){
		return false;
	}
	uint32 dwSize = qfsize(hFile);
	char* pMap = (char*)malloc(dwSize);
	eread(hFile, pMap, dwSize);

	qstring Sig = pMap;
	qstring Config = GetMidString(Sig, "******Config******\r\n", "******Config_End******", 0);
	if (Config.find("IsAligned=true") != -1) {
		m_IsAligned = true;
	}
	else
	{
		m_IsAligned = false;
	}

	if (Config.find("IsAllMem=true") != -1) {
		m_IsAllMem = true;
	}
	else
	{
		m_IsAllMem = false;
	}
	
	if (Config.find("IsSetName=false") != -1) {
		m_IsSetName = false;
	}
	else
	{
		m_IsSetName = true;
	}


	return true;
}