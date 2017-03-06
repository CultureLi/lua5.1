#pragma  once
#include "stdafx.h"
#include "ScriptMgr.h"

ScriptMgr::ScriptMgr()
{

}

ScriptMgr::~ScriptMgr()
{

}

ScriptMgr& ScriptMgr::Instance()
{
	static ScriptMgr mgr;
	return mgr;
}
