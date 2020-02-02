# DBSessionDefiner
Define a DB table struct with c++ macro definition, then you can use data object and session method to query/insert/update and so on. Things will be easier if c++ supports reflect


# Example

You can define a database table with following code.
``` c++
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
		ID,                     // record id
		Name,                   // user name
		Flags,                  // flags
		Par1,                   // append param 1
		Par2,                   // append param 2

		_MAX_COUNT
	};

	template<typename _ConnType>
	using DBFetch = DAO::DefDBFetcher<_ConnType>;

	BeginDefineDBMembers("TestTable1");                 // table name

	DefineMemberDBType(ID, int, "id");                  // db column names
	DefineMemberDBType(Name, std::string, "name");
	DefineMemberDBType(Flags, std::bitset<32>, "flags");
	DefineMemberDBType(Par1, int, "par1");
	DefineMemberDBType(Par2, int, "par2");

	FinishDefineDBMembers(_MAX_COUNT);
}; 

#include "GeneralDBSession/GeneralDBSession.h"

using TestTableDBSession = DAO::CGeneralDBSession<TestTableDBData, 0>;
```


Then you can easily generate sql with following code.

```c++

	bool b_found;
	{
		TestTableDBData data;
		TestTableDBSession::GetDataByID(data, b_found, 1);                
          // print  SELECT id,name,flags,par1,par2 FROM TestTable1 WHERE id=1;
	}

	{
		TestTableDBData data;
		using Attrs = TestTableDBData::Attributes;
		TestTableDBSession::GetDataSomeAttribByID<Attrs::Flags, Attrs::Par2>(data, b_found, 1);
         // print  SELECT flags,par2 FROM TestTable1 WHERE id=1;
	}

	{
		TestTableDBData data(11, "g'22", bitset<32>("11001"), 44, 55);
		TestTableDBSession::InsertData(data);
        // print  INSERT INFO TestTable1(id,name,flags,par1,par2) VALUES(11,'g''22', b'11001', 44, 55);
	}
```

encapsulate a DB operate Class, then you can hide all details behind DAO classes. It's possible to support different db type.


It's obvious that some difficult sql-usage requires inheritance and extension, most common usage requires no specific code.