#pragma once
#include "GeneralDBSession/GeneralDBSession.h"

class CConnect;
namespace  DAO
{
	using namespace std;

	namespace DAO_FUNCS
	{
		template<typename _DataType,typename _ConnType , typename ...Arg>
		void FetchDataSToInsertC(function<void(int, const string&, const string&)> func_append, _ConnType& res, const vector<DBParamDefType>& vs_attrs, const tuple<Arg...>& data)
		{
			DAO_FUNCS::FetchDatasToInsert<_DataType>(func_append, res, vs_attrs, data);

			for (size_t i = 0; i < vs_attrs.size(); i++)
			{
				auto&attr = vs_attrs[i];
				if (get<1>(attr) == 1)
				{
					func_append(i, get<0>(attr), get<2>(attr));
				}
			}
		}
	}

	template <typename _DataType, size_t IDPOS>
	bool CGeneralDBSession<_DataType, IDPOS>::Init()
	{
		if (b_inited)
		{
			return true;
		}
		vs_defines = _DataType::GetDBTypes();
		for (size_t i = 0; i < vs_defines.size(); i++)
		{
			if (get<1>(vs_defines[i]) == 0)
			{
				if (!needed_params.empty()) needed_params += ',';
				needed_params += '"'+get<0>(vs_defines[i])+'"';
			}
		}
		b_inited = true;
		return true;
	}

	template <typename _DataType, size_t IDPOS>
	bool CGeneralDBSession<_DataType, IDPOS>::GetDataByID(_DataType& data, bool& b_found, ID id)
	{
		Init();
		static_assert(IDPOS >= 0, "[DAO] This DBSession Not Defined ID pos!");
		CConnect res;
		string str = SqlConds<CConnect>::SqlCondOP_Pos<IDPOS>("=", id).ToString();
		string strSql = "SELECT " + needed_params + " FROM \"" + _DataType::DBTableName() + "\" WHERE \"" + get<0>(vs_defines[IDPOS]) +
			"\"=" + DAO_FUNCS::GetFetchToPos<_DataType, decltype(res), decltype(get<IDPOS>(data))>(IDPOS, id) + ";";
		//res.DoSql()
		printf("sql:\t%s\n", strSql.c_str());
		//if(found)
		{
			DAO_FUNCS::FetchFromDatas<_DataType>(res, data);
			b_found = true;
		}
		return true;
	}

	template <typename _DataType, size_t IDPOS>
	bool CGeneralDBSession<_DataType, IDPOS>::InsertData(_DataType& data)
	{
		Init();
		auto vs_attrs = vs_defines;
#ifdef DAO_INSERT_WITH_ID
#else
		if (IDPOS >= 0)
		{
			get<2>(vs_attrs[IDPOS]) = 3;
		}
#endif
		string names, values;
		auto func_append = [&](int index, const string& name, const string& value)
		{
			if (!names.empty())names += ',';
			names += '"' + name + '"';
			if (!values.empty())values += ',';
			values += value ;
		};

		CConnect res;
		DAO_FUNCS::FetchDataSToInsertC<_DataType>(func_append, res, vs_attrs, data);
		 

		string strSql = "INSERT INTO \"" + _DataType::DBTableName() + "\" (" + names + ") VALUES(" + values + ");";


		//res.DoSql()
		printf("sql:\t%s\n", strSql.c_str());

		return true;
	}

	template <typename _DataType, size_t IDPOS>
	template <int... Atts>
	bool CGeneralDBSession<_DataType, IDPOS>::GetDataSomeAttribByID(_DataType& data, bool b_found, ID id)
	{
		vector<int> attrs = DAO_FUNCS::ExpandArgToVector<Atts...>();
		return GetDataSomeAttribByID(data, b_found, id, attrs);
	}

	template <typename _DataType, size_t IDPOS>
	bool CGeneralDBSession<_DataType, IDPOS>::GetDataSomeAttribByID(_DataType& data, bool b_found, ID id,
		const vector<int>& attrs)
	{
		Init();
		static_assert(IDPOS >= 0, "[DAO] This DBSession Not Defined ID pos!");
		string name;
		for (int ipos : attrs)
		{
			if (!name.empty()) name += ',';
			name += '"'+get<0>(vs_defines[ipos])+'"';
		}
		CConnect res;
		string strSql = "SELECT " + name + " FROM \"" + _DataType::DBTableName() + "\" WHERE \"" + get<0>(vs_defines[IDPOS]) + "\"=" +
			DAO_FUNCS::GetFetchToPos<_DataType, decltype(res), decltype(get<IDPOS>(data))>(IDPOS, id) + ";";
		//res.DoSql()
		printf("sql:\t%s\n", strSql.c_str());
		//if(found)
		{
			bitset<_DataType::_PARAM_NUM> req;
			for (int ipos : attrs) { req.set(ipos); }
			DAO_FUNCS::FetchFromDatasByReq<_DataType>(res, req, data);
			b_found = true;
		}
		return true;
	}



	string ReplaceStr(const string& from, char find, const string& to)
	{
		string too;
		size_t lastpos = string::npos;
		size_t pos = from.find_first_of(find);
		while (pos != string::npos)
		{
			if (pos - lastpos > 1)
			{
				too.append(from.substr(lastpos == string::npos ? 0 : lastpos + 1, pos - lastpos - 1));
			}
			too.append(to);
			lastpos = pos;
			pos = from.find_first_of(find, lastpos + 1);
		}
		if (lastpos != from.size() - 1)
		{
			too.append(from.substr(lastpos + 1));
		}
		return std::move(too);
	}


	template<>
	class DefDBFetcher<CConnect>
	{
	public:


		template<typename _Type>
		static void FetchFrom(CConnect& conn, int index, _Type& param)
		{
		}
		template<typename _Type>
		static void FetchFrom(CConnect& conn, const string& name, _Type& param)
		{
		}


		template<typename _Type>
		static void FetchTo(string& val, int index, const _Type& param)
		{
			val = '\'' + std::to_string(param) + '\'';
		}
		template<typename _Type>
		static void FetchTo(string& val, const string& name, const _Type& param)
		{
			val = '\'' + std::to_string(param) + '\'';
		}


		static void FetchTo(string& val, int index, const string& param)
		{
			val = '\'' + ReplaceStr(param,'\'',"''") + '\'';
		}
		static void FetchTo(string& val, const string& name, const string& param)
		{
			val = '\'' + ReplaceStr(param, '\'', "''") + '\'';
		}

		template<size_t bits>
		static void FetchTo(string& val, int index, const bitset<bits>& param)
		{
			val = '\'' + param.to_string() + '\'';
		}
		template<size_t bits>
		static void FetchTo(string& val, const string& name, const bitset<bits>& param)
		{
			val = '\'' + param.to_string() + '\'';
		}





		static void FetchFrom(CConnect& conn, const string& name, int& param)
		{
			param = 123; // conn.getint
		}
		static void FetchFrom(CConnect& conn, const string& name, ID& param)
		{
			param = 456; // conn.getuint
		}
		static void FetchFrom(CConnect& conn, const string& name, string& param)
		{
			param = "789"; // conn.getstr
		}

		template<size_t bits>
		static void FetchFrom(CConnect& conn, const string& name, bitset<bits>& param)
		{
			param = bitset<bits>("10101");//conn.getstring
		}

	};
}