#pragma once
#include <string>
#include <vector>
#include <functional>

namespace DAO {

	struct SqlCond
	{
		SqlCond() {}
		virtual ~SqlCond() {}
		virtual std::string ToString()const { return ""; }
		virtual std::string ToString(const std::vector<std::string>&)const { return ""; }
		// 1 ºãÕæ TRUE  -1 ºã¼Ù FALSE  0 ÐèÖ´ÐÐÅÐ¶Ï
		virtual int IsConst() const { return -1; }
		operator std::string() const
		{
			return ToString();
		}
	};

	static inline std::string to_string(const SqlCond& sql_)
	{
		return sql_.ToString();
	}


	struct SqlCondStr : std::string, SqlCond
	{
		SqlCondStr(const std::string& s) :std::string(s) {}
		SqlCondStr(const std::string& s, int i_const) :std::string(s), i_const_(i_const) {}
		std::string ToString()const override { return *(std::string*)this; }
		int IsConst() const override { return i_const_; }
	private:
		int i_const_ = 0;
	};

	struct SqlCondSame : SqlCond
	{
		SqlCondSame() {}
		SqlCondSame(const std::string& p_const, int const_bool) :pin_(new SqlCondStr(p_const, const_bool)), b_need_del_(true) {}
		SqlCondSame(SqlCondSame&& sql) :pin_(sql.pin_), b_need_del_(sql.b_need_del_) { sql.pin_ = nullptr; sql.b_need_del_ = false; }
		SqlCondSame(const SqlCond& p_const) :pin_(new SqlCondStr(p_const.ToString(), p_const.IsConst())), b_need_del_(true) {}
		SqlCondSame(const SqlCond* in, bool need_del = true) :pin_(in), b_need_del_(need_del) {}
		~SqlCondSame() { Clear(); }
		std::string ToString()const override { if (pin_ == nullptr)return ""; return pin_->ToString(); }
		int IsConst() const override { if (pin_ == nullptr)return -1; return pin_->IsConst(); }
		void Clear()
		{
			if (b_need_del_ && pin_ != nullptr)delete pin_; pin_ = nullptr;
		}
		void Set(const std::string& p_const, int const_bool)
		{
			Clear();
			pin_ = new SqlCondStr(p_const, const_bool); b_need_del_ = true;
		}
		void Set(const SqlCond& p_const) 
		{
			Clear();
			pin_ = new SqlCondStr(p_const.ToString(), p_const.IsConst()); b_need_del_ = true;
		}
		void Set(SqlCondSame&& sql)
		{
			Clear();
			pin_ = sql.pin_; 
			b_need_del_ = sql.b_need_del_;
			sql.pin_ = nullptr; 
			sql.b_need_del_ = false;
		}
		
		void Set(const SqlCond* in, bool need_del = true) 
		{
			Clear();
			pin_ = in; b_need_del_=need_del;
		}
		SqlCondSame& operator=(SqlCondSame&& sql)
		{
			Set(sql);
			return *this;
		}
	private:
		const SqlCond * pin_ = nullptr;
		bool b_need_del_ = false;
	};

	struct SqlCondNOT : SqlCond
	{
		SqlCondNOT(const SqlCond& tmp) :in_(tmp)
		{
		}
		SqlCondNOT(const SqlCond* in, bool need_del = true) :in_(in, need_del)
		{
		}
		~SqlCondNOT() = default;

		std::string ToString()const override
		{
			const auto i = IsConst();
			if (i != 0)
			{
				return i > 0 ? "TRUE" : "FALSE";
			}
			std::string tmp = in_.ToString();
			if (tmp.empty())
			{
				return "";
			}
			return "NOT " + tmp;
		}
		int IsConst() const override
		{
			int i_c = in_.IsConst();
			if (i_c != 0)
			{
				return (i_c > 0) ? -1 : 1;
			}
			return 0;
		}
	private:
		SqlCondSame in_;
	};

	struct SqlCondOR :SqlCond
	{
		SqlCondOR(const SqlCond& in, const SqlCond& in2) :in_(in), in2_(in2) {}
		SqlCondOR(const SqlCond* in, const SqlCond*in2, bool b_need_del) :in_(in, b_need_del), in2_(in2, b_need_del) {}
		~SqlCondOR() = default;
		int IsConst() const override
		{
			int i = in_.IsConst(), j = in2_.IsConst();
			if (i > 0 || j > 0)
			{
				return 1;
			}
			if (i < 0 && j < 0)
			{
				return -1;
			}
			return 0;
		}
		std::string ToString() const override
		{
			int m = IsConst();
			if (m != 0)
			{
				return m > 0 ? "TRUE" : "FALSE";
			}
			int i = in_.IsConst(), j = in2_.IsConst();
			if (i < 0)
			{
				return in2_.ToString();
			}
			if (j < 0)
			{
				return in_.ToString();
			}
			std::string t1 = in_.ToString(), t2 = in2_.ToString();
			if (t1.empty())
			{
				return t2;
			}
			if (t2.empty())
			{
				return t1;
			}
			return "(" + t1 + " OR " + t2 + ")";
		}
	private:
		SqlCondSame in_;
		SqlCondSame in2_;
	};

	struct SqlCondAND :SqlCond
	{
		SqlCondAND(const SqlCond& in, const SqlCond&in2) :in_(in), in2_(in2) {}
		SqlCondAND(const SqlCond* in, const SqlCond*in2, bool b_need_del) :in_(in, b_need_del), in2_(in2, b_need_del) {}
		~SqlCondAND() = default;
		int IsConst() const override
		{
			int i = in_.IsConst(), j = in2_.IsConst();
			if (i > 0 && j > 0)
			{
				return 1;
			}
			if (i < 0 || j < 0)
			{
				return -1;
			}
			return 0;
		}
		std::string ToString() const override
		{
			int m = IsConst();
			if (m != 0)
			{
				return m > 0 ? "TRUE" : "FALSE";
			}
			int i = in_.IsConst(), j = in2_.IsConst();
			if (i > 0)
			{
				return in2_.ToString();
			}
			if (j > 0)
			{
				return in_.ToString();
			}
			std::string t1 = in_.ToString(), t2 = in2_.ToString();
			if (t1.empty() || t2.empty())
			{
				return "FALSE";
			}
			return "(" + t1 + " AND " + t2 + ")";
		}
	private:
		SqlCondSame in_;
		SqlCondSame in2_;
	};

	struct SqlCondFunc :SqlCond
	{
		SqlCondFunc(const std::function<std::pair<std::string, int>()>&var_) :func_(var_) {}
		std::string ToString() const override
		{
			return func_().first;
		}
		int IsConst() const override
		{
			return func_().second;
		}
	private:
		std::function<std::pair<std::string, int>()> func_;
	};

	struct SqlCondIN :SqlCond
	{
		template<typename _T>
		SqlCondIN(const std::string& var_name, const _T& list)
		{
			if (!var_name.empty() && !list.empty())
			{
				std::string strtmp;
				for (auto& c : list)
				{
					if (!strtmp.empty())strtmp += ',';
					strtmp += to_string(c);
				}
				std::string str = var_name + " IN(" + strtmp + ")";
				int is_const = 0;
				in_ = SqlCondSame(str, is_const);
			}
			else if (list.empty())
			{
				in_ = SqlCondSame("FALSE", -1);
			}
			else if (var_name.empty())
			{
				in_ = SqlCondSame("TRUE", 1);
			}
		}
		template<typename _T>
		SqlCondIN(const std::string& var_name, const _T* list)
		{
			if (!var_name.empty() && list != nullptr)
			{
				auto func_ = [=]()
				{
					const _T *list2 = list;
					if (!list2->empty())
					{
						std::string strtmp;
						for (auto& c : *list2)
						{
							if (!strtmp.empty())strtmp += ',';
							strtmp += to_string(c);
						}
						std::string str = var_name + " IN(" + strtmp + ")";
						int is_const = 0;
						return make_pair(str, is_const);
					}
					else if (list2->empty())
					{
						return make_pair(std::string("FALSE"), (int)-1);
					}
				};
				in_ = SqlCondSame(new SqlCondFunc(func_));
			}
			else if (list == nullptr)
			{
				in_ = SqlCondSame("FALSE", -1);
			}
			else
			{
				in_ = SqlCondSame("TRUE", 1);
			}
		}
		std::string ToString() const override
		{
			return in_.ToString();
		}
		int IsConst() const override
		{
			return in_.IsConst();
		}

	protected:
		static inline std::string to_string(const std::string& str)
		{
			return str;
		}
		static inline std::string to_string(const char* str)
		{
			return str;
		}
	private:
		SqlCondSame in_;
	};

	struct SqlCondOP :SqlCond
	{
		template<typename _T>
		SqlCondOP(const std::string& var_name, const std::string&op, const _T& var_, bool blank_val_true = false)
		{
			std::string var_str = to_string(var_);
			if (!var_name.empty() && !var_str.empty())
			{
				std::string str_sql_ = var_name + " " + op + " " + var_str;
				int is_const = 0;
				in_ = SqlCondSame(str_sql_, is_const);
			}
			else if (var_str.empty())
			{
				in_ = SqlCondSame(blank_val_true ? "TRUE" : "FALSE", blank_val_true ? 1 : -1);
			}
			else if (var_name.empty())
			{
				in_ = SqlCondSame("TRUE", 1);
			}
		}

		template<typename _T>
		SqlCondOP(const std::string& var_name, const std::string&op, const _T* var_, bool blank_val_true = false)
		{
			if (!var_name.empty())
			{
				auto func_ = [=]() {
					const _T* var2_ = var_;
					std::string var_str = to_string(var2_);
					if (!var_str.empty())
					{
						std::string str_sql_ = var_name + " " + op + " " + var_str;
						int is_const = 0;
						return make_pair(str_sql_, is_const);
					}
					else
					{
						return make_pair(std::string(blank_val_true ? "TRUE" : "FALSE"), blank_val_true ? 1 : -1);
					}
				};
				in_ = SqlCondSame(new SqlCondFunc(func_));
			}
			else
			{
				in_ = SqlCondSame("TRUE", 1);
			}
		}
		std::string ToString() const override
		{
			return in_.ToString();
		}
		int IsConst() const override
		{
			return in_.IsConst();
		}

	protected:
		static inline std::string to_string(const std::string& str)
		{
			return str;
		}
		static inline std::string to_string(const char* str)
		{
			return str;
		}
	private:
		SqlCondSame in_;
	};


}