#pragma once
#include "GeneralDBSession/DBSessionDataDefiner.h"
#include <bitset>
#include "GeneralDBSessionImplCConnect.h"


struct TestTableDBData : public std::tuple<ID, std::string, std::bitset<32>, int ,int >
{
public:
	using BaseTupleType = tuple<ID, std::string, std::bitset<32>, int, int>;
	using BaseTupleType = BaseTupleType;

	TestTableDBData(BaseTupleType&& data) :BaseTupleType(data) {}
	TestTableDBData(const BaseTupleType& data):BaseTupleType(data){}
	TestTableDBData(ID dbid, const std::string& name, const std::bitset<32>&flags, int par1, int par2)
		:BaseTupleType(dbid, name, flags, par1, par2)
	{}
	enum Attributes
	{
		DBID,
		Name,
		Flags,
		Par1,
		Par2,

		_MAX_COUNT
	};

	template<typename _ConnType>
	using DBFetch = DAO::DefDBFetcher<_ConnType>;

	BeginDefineDBMembers("TestTable1");

	DefineMemberDBType(DBID, ID, "id");
	DefineMemberDBType(Name, std::string, "name");
	DefineMemberDBType(Flags, std::bitset<32>, "flags");
	DefineMemberDBType(Par1, int, "par1");
	DefineMemberDBType(Par2, int, "par2");

	FinishDefineDBMembers(_MAX_COUNT);
};




#include "GeneralDBSession/GeneralDBSession.h"

using TestTableDBSession = DAO::CGeneralDBSession<TestTableDBData, 0>;