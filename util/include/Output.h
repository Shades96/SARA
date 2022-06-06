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

	struct Debug
	{
		static Debug& inst()
		{
			static Debug i;
			return i;
		}
		Debug& operator<<(const string& str)
		{
			//std::cout << str;
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
	static Debug& debug()
	{
		return Output::Debug::inst();
	}
	static Error& error()
	{
		return Output::Error::inst() << "[ERROR] ";
	}
	static Error& error(int lineNum)
	{
		return Output::error() << "Line " << std::to_string(lineNum) << ": ";
	}
};
