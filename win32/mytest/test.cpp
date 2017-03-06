#include "stdafx.h"
#include <stdio.h>
#include "test.h"

T::T()
{
	data = 2;
}

T& T::Instance()
{
	static T ins;
	return ins;
}

void T::SetData(int value)
{
	data = value;
}

int T::GetData()
{
	return data;
}

void T::Show()
{
	 printf("Hello tolua");
}