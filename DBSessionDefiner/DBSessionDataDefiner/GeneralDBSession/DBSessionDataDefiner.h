#pragma once
#include <tuple>
#include <string>

typedef std::tuple<std::string, int, std::string> DBParamDefType;

	// 定义数据表成员开始
#define BeginDefineDBMembers(_DB_TABLE_NAME) \
	static std::string DBTableName(){return _DB_TABLE_NAME;}\
	protected:\
		template<unsigned n>\
		struct ParamType{\
			static DBParamDefType GetDBType() { return DBParamDefType("", -1, ""); }\
		};
		
#define DefineMemberDBType(_NAME, _TYPE, _DBNAME) DefineMemberGetterSetter(Attributes::_NAME, _NAME, _TYPE, _DBNAME);

#ifdef DAO_FRIENDLY_MEMBERNAME
#define DEF_REF(_POS, P_NAME, _TYPE) _TYPE& P_NAME = get<_POS>(*static_cast<BaseTupleType*>(this));
#else
#define DEF_REF(...)
#endif

#define DefineMemberGetterSetter(_POS, _NAME, _TYPE, _DBNAME) \
	public:\
		inline const _TYPE& Get##_NAME() const { return std::get<_POS>(*static_cast<const BaseTupleType*>(this)); }\
			inline _TYPE& Get##_NAME() { return std::get<_POS>(*static_cast<BaseTupleType*>(this)); }\
			inline void Set##_NAME(const _TYPE&val){std::get<_POS>(*static_cast<BaseTupleType*>(this))=val;}\
			DEF_REF(_POS, _NAME, _TYPE);\
	protected:\
		template<>\
		struct ParamType<_POS>\
		{\
			static DBParamDefType GetDBType(){return DBParamDefType(_DBNAME, 0, "");}\
		};


#define DefineExternDBType(_POS, _DBNAME, _DEFVALUE) \
	protected:\
		template<>\
		struct ParamType<_POS>\
		{\
			static DBParamDefType GetDBType() { return DBParamDefType(_DBNAME, 1, _DEFVALUE); }\
		};

#define FinishDefineDBMembers(_NUM) \
	public:\
		static const int _PARAM_NUM = _NUM;\
	private:\
		template<unsigned n>\
		struct ParamTypeProxy\
		{\
			static void PushDBType(std::vector<DBParamDefType>& vs_params){\
				ParamTypeProxy<n-1>::PushDBType(vs_params);\
				vs_params.push_back(ParamType<n>::GetDBType());\
			}\
		};\
		template<>\
		struct ParamTypeProxy<-1>\
		{\
			static void PushDBType(std::vector<DBParamDefType>& vs_param){}\
		};\
	public:\
		static std::vector<DBParamDefType> GetDBTypes()\
		{\
			std::vector<DBParamDefType> vs_types;\
			ParamTypeProxy<_NUM - 1>::PushDBType(vs_types);\
			return std::move(vs_types);\
		}
