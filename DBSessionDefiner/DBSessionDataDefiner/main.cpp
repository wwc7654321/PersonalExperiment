// DBSessionDataDefiner.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
class CConnect
{
	
};
#include "DAO/TestTableDBSession.h"

using namespace  std;
int main()
{
	bool b_found;
	
	
	string val; ID id;
	using DBFetch1 = TestTableDBData::DBFetch<CConnect>;
	DBFetch1::FetchTo(val, 1, id);

	{
		TestTableDBData data(11, "g'22", bitset<32>("11001"), 44, 55);
		TestTableDBSession::GetDataByID(data, b_found, 1);
	}

	{
		TestTableDBData data(11, "g'22", bitset<32>("11001"), 44, 55);
		using Attrs = TestTableDBData::Attributes;
		TestTableDBSession::GetDataSomeAttribByID<Attrs::Flags, Attrs::Par2>(data, b_found, 1);
	}

	{
		TestTableDBData data(11, "g'22", bitset<32>("11001"), 44, 55);
		TestTableDBSession::InsertData(data);
	}


	getchar();
    return 0;
}

