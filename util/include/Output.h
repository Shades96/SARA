#pragma once

#include "Instruction.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

using std::string;

struct Output
{
	struct Print
	{
		static Print& inst()
		{
			static Print i;
			return i;
		}
		Print& operator<<(const string& str)
		{
			std::cout 

				#ifndef NDEBUG
				<< "> "
				#endif

				<< str

				#ifndef NDEBUG
				<< "\n"
				#endif
				;
			return *this;
		}
	};
	static Print& print()
	{
		return Output::Print::inst();
	}

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
	static Log& log()
	{
		return Output::Log::inst();
	}

	struct Debug
	{
		static Debug& inst()
		{
			static Debug i;
			return i;
		}
		Debug& operator<<(const string& str)
		{
			#ifndef NDEBUG
			std::cout << str;
			#endif
			return *this;
		}
	};
	static Debug& debug()
	{
		return Output::Debug::inst();
	}

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
	static Error& error()
	{
		return Output::Error::inst() << "[ERROR] ";
	}
	static Error& error(int lineNum)
	{
		return Output::error() << "Line " << std::to_string(lineNum) << ": ";
	}

	struct Bytecode
	{
		static void setOutfile(const string& filename)
		{
			inst().out = std::make_shared<std::ofstream>(filename, std::ios::binary | std::ios::out);
		}
		static void closeOutfile()
		{
			inst().out->close();
			inst().out = nullptr;
		}
		static Bytecode& inst()
		{
			static Bytecode i;
			return i;
		}
		Bytecode& operator<<(const Instruction& instr)
		{
			if (!out) {
				Output::error() << "Bytecode output channel is not initialized\n";
				return *this;
			}

			#ifndef NDEBUG
			if (inst().bufStack.size() == 1) {
				Output::debug() << instr.to_string() << "\n";
			}
			#endif

			bufStack.back().push_back(instr);
			return *this;
		}
		Bytecode& operator<<(vector<Instruction> instrs)
		{
			for (auto& i : instrs) {
				Output::code() << i;
			}
			return *this;
		}
		static void flush() {
			if (inst().bufStack.size() != 1) {
				Output::error() << "Buffer stack has not been initialized or flattened\n";
				return;
			}

			for (auto& instr : inst().bufStack[0]) {
				auto bytes = instr.toBytes();
				auto numBytes = instr.numConstOperands * sizeof(operand) + sizeof(operation);
				inst().out->write(bytes.data(), numBytes);
			}
			
			inst().bufStack.clear();
			inst().bufStack = vector<vector<Instruction>>(1);
		}
		static void push() {
			inst().bufStack.push_back(vector<Instruction>{ });
		}
		static vector<Instruction> pop() {
			auto top = inst().bufStack.back();
			inst().bufStack.pop_back();
			return top;
		}
	private:
		vector<vector<Instruction>> bufStack{ vector<vector<Instruction>>(1) };
		std::shared_ptr<std::ofstream> out;
	};
	static Bytecode& code()
	{
		return Output::Bytecode::inst();
	}
};
