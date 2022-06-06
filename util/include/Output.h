#pragma once

#include <iostream>
#include <string>

using std::string;

struct Output
{
	struct Bytecode
	{
		static Bytecode& inst()
		{
			static Bytecode i;
			return i;
		}
		Bytecode& operator<<(const string& str)
		{
			return *this;
		}
	};

	struct Log
	{
		static Log& inst()
		{
			static Log i;
			return i;
		}
		Log& operator<<(const string& str)
		{
			std::cout << str;
			return *this;
		}
	};

	struct Error
	{
		static Error& inst()
		{
			static Error i;
			return i;
		}
		Error& operator<<(const string& str)
		{
			std::cout << str;
			return *this;
		}
	};

	static Bytecode& code()
	{
		return Output::Bytecode::inst();
	}
	static Log& log()
	{
		return Output::Log::inst();
	}
	static Error& error()
	{
		return Output::Error::inst();
	}
};
