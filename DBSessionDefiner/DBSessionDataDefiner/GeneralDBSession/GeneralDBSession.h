#pragma once
#include "DBSessionDataDefiner.h"
#include "SqlCond.h"
#include<string>
#include<vector>
#include<bitset> 
#include <functional>

namespace DAO
{
	using namespace std;
	namespace DAO_FUNCS
	{
		template<typename _T>
		void ExpandArgToVectorI(vector<int>& vs)
		{ 
		}

		template<typename _T, int Arg, int ...Args>
		void ExpandArgToVectorI(vector<int>& vs)
		{
			vs.push_back(Arg);
			ExpandArgToVectorI<_T, Args...>(vs);
		}

		template<int ...Args>
		vector<int> ExpandArgToVector()
		{
			vector<int> vs;
			ExpandArgToVectorI<void, Args...>(vs);
			return std::move(vs);
		}


		template<typename _DataType, typename _ConnType, typename Args>
		void FetchFromDatasI(_ConnType& conn, int index, tuple<Args>& tp)
		{
			_DataType::DBFetch<_ConnType>::FetchFrom(conn, index, get<0>(tp));
		}
		template<typename _DataType, typename _ConnType, typename Arg, typename ...Args>
		void FetchFromDatasI(_ConnType& conn, int index, tuple<Arg, Args...>& tp)
		{
			_DataType::DBFetch<_ConnType>::FetchFrom(conn, index, get<0>(tp));
			FetchFromDatasI<_DataType>(conn, index+1, (tuple<Args...>&)tp);
		}

		template<typename _DataType, typename _ConnType, typename ...Args>
		void FetchFromDatas(_ConnType& conn, tuple<Args...>& tp)
		{
			FetchFromDatasI<_DataType>(conn, 0, tp);
		}


		template<typename _DataType, typename _ConnType, size_t maxs, typename Arg>
		void FetchFromDatasByReqI(_ConnType& conn, const bitset<maxs>&req, int index, tuple<Arg>& tp)
		{
			if (req.test(index))_DataType::DBFetch<_ConnType>::FetchFrom(conn, index, get<0>(tp));
		}
		template<typename _DataType, typename _ConnType, size_t maxs, typename Arg, typename ...Args>
		void FetchFromDatasByReqI(_ConnType& conn, const bitset<maxs>&req, int index, tuple<Arg, Args...>& tp)
		{
			if (req.test(index))_DataType::DBFetch<_ConnType>::FetchFrom(conn, index, get<0>(tp));
			FetchFromDatasByReqI<_DataType, _ConnType, maxs>(conn, req, index + 1, (tuple<Args...>&)tp);
		}

		template<typename _DataType, typename _ConnType, size_t maxs, typename ...Args>
		void FetchFromDatasByReq(_ConnType& conn, const bitset<maxs>&req, tuple<Args...>& tp)
		{
			FetchFromDatasByReqI<_DataType, _ConnType, maxs>(conn, req, 0, tp);
		}


		template<typename _DataType, typename _ConnType, typename _Type>
		string GetFetchToName(const string& name, _Type& v)
		{
			string val;
			_DataType::DBFetch<_ConnType>::FetchTo(val, name, v);
			return std::move(val);
		}
		template<typename _DataType, typename _ConnType, typename _Type>
		string GetFetchToPos(int pos, _Type& v)
		{
			string val;
			_DataType::DBFetch<_ConnType>::FetchTo(val, pos, v);
			return std::move(val);
		}


		template<int index, typename _DataType, typename _ConnType, typename Arg>
		void FetchDatasToInsertI(const function<void(int,const string&, const string&)>& func_callback, _ConnType& conn,
			const vector<DBParamDefType>& attrs, const tuple<Arg>& tp)
		{
			if (get<1>(attrs[index]) == 0)
			{
				func_callback(index,get<0>(attrs[index]), GetFetchToPos<_DataType, _ConnType>(index, get<0>(tp)));
			}
		}
		template<int index, typename _DataType, typename _ConnType, typename Arg, typename ...Args>
		void FetchDatasToInsertI(const std::function<void(int, const string&, const string&)>& func_callback, _ConnType& conn,
			const vector<DBParamDefType>& attrs, const tuple<Arg, Args...>& tp)
		{
			FetchDatasToInsertI<index, _DataType, _ConnType, Arg>(func_callback, conn, attrs, (tuple<Arg>&)get<0>(tp));
			int a = 1;
			FetchDatasToInsertI<index+1, _DataType, _ConnType, Args...>(func_callback, conn, attrs, (tuple<Args...>&)tp);
		}
		template<typename _DataType, typename _ConnType,typename ...Args>
		void FetchDatasToInsert(std::function<void(int, const string&, const string&)> func_callback, _ConnType& conn, const vector<DBParamDefType>& attrs, const tuple<Args...>& tp)
		{
			FetchDatasToInsertI<0, _DataType, _ConnType, Args...>(func_callback, conn, attrs, tp);
		}
	}

	template<typename _DB_DataType, int id_pos>
	struct SqlCondIN_P :public SqlCondIN
	{
		template<typename _T>
		SqlCondIN_P(const _T& list) :SqlCondIN(std::get<0>(_DB_DataType::template ParamType<id_pos>::GetDBType()), list)
		{
		}
		SqlCondIN_P(const std::string& list) : SqlCondIN(std::get<0>(_DB_DataType::template ParamType<id_pos>::GetDBType()), list)
		{
		}
	};



	template<typename _DB_DataType, int id_pos>
	struct SqlCondOP_P :public SqlCondOP
	{
		struct DataType:_DB_DataType
		{
			friend struct SqlCondOP_P;
		};
		template<typename _T>
		SqlCondOP_P(const std::string& op, const _T& value) :SqlCondOP(std::get<0>(DataType::template ParamType<id_pos>::GetDBType()), op, value)
		{
		}
		SqlCondOP_P(const std::string& op, const std::string& value) : SqlCondOP(std::get<0>(DataType::template ParamType<id_pos>::GetDBType()), op, value)
		{
		}
	};




	template<typename _DataType, size_t IDPOS = -1>
	class CGeneralDBSession
	{
	public:
		static bool Init();

		static bool GetDataByID(_DataType& data, bool& b_found, ID id);

		static bool InsertData(_DataType& data);

		template<int ...Atts>
		static bool GetDataSomeAttribByID(_DataType& data, bool b_found, ID id);

		static bool GetDataSomeAttribByID(_DataType& data, bool b_found, ID id, const vector<int>& attrs);

		template<typename _ConnType>
		struct SqlConds {
			template<int pos>
			struct SqlCondIN_Pos : SqlCondIN_P<_DataType, pos>
			{
				//typedef tuple_element_t<pos, typename _DataType::BaseTupleType> _Type;
				typedef decltype(get<pos>(_DataType::BaseTupleType())) _Type;
				SqlCondIN_Pos(const vector<_Type>& value) :SqlCondIN_P<_DataType, pos>(JoinFetchedToPos<_DataType, _ConnType>(pos, value))
				{
					static_assert(pos >=0 && pos <= _DataType::_PARAM_NUM, "Invalid ID Pos");
				}
				SqlCondIN_Pos(const string& value) :SqlCondIN_P(value) {}
			};

			template<int pos>
			struct SqlCondOP_Pos : SqlCondOP_P<_DataType, pos>
			{
				//typedef tuple_element_t<pos, typename _DataType::BaseTupleType> _Type;
				typedef decltype(get<pos>(_DataType::BaseTupleType())) _Type;
				SqlCondOP_Pos(const string& op, const _Type& value) :SqlCondOP_P<_DataType, pos>(op, DAO_FUNCS::GetFetchToPos<_DataType, _ConnType>(pos, value))
				{
					static_assert(pos >= 0 && pos <= _DataType::_PARAM_NUM, "Invalid ID Pos");
				}
				SqlCondOP_Pos(const string& op, const string& value) :SqlCondOP_P(op, value) {}
			}; 
			using SqlCond = SqlCond;
			using SqlCondAND = SqlCondAND;
			using SqlCondOR = SqlCondOR;
		};

		static bool b_inited;
		static vector<DBParamDefType> vs_defines;
		static string needed_params;
	};

	template<typename _ConnType>
	class DefDBFetcher
	{
		template<typename _Type>
		static void FetchFrom(_ConnType& conn, int index, _Type& param)
		{
		}
		template<typename _Type>
		static void FetchFrom(_ConnType& conn, const string& name, _Type& param)
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
			val = '\'' + param + '\'';
		}
		static void FetchTo(string& val, const string& name, const string& param)
		{
			val = '\'' + param + '\'';
		}
	};

	template<typename _DataType, size_t IDPOS>
	bool CGeneralDBSession<_DataType, IDPOS>::b_inited = false;

	template<typename _DataType, size_t IDPOS>
	vector<DBParamDefType> CGeneralDBSession<_DataType, IDPOS>::vs_defines;

	template<typename _DataType, size_t IDPOS>
	string CGeneralDBSession<_DataType, IDPOS>::needed_params;
}
