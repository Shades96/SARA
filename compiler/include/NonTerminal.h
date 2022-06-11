#pragma once

#include "Terminal.h"
#include "Output.h"

class NonTerminal
{
public:
	NonTerminal(vector<NonTerminal> pattern) : 
		pattern(pattern)
	{}
private:
	vector<NonTerminal> pattern;
};

class Optional : public NonTerminal
{

};

class Repeat : public NonTerminal
{

};

class ParenthesisPair
{
public:
	int parse(Terminal& t)
	{
		if (!stack.empty()) {
			auto err = stack.back().parse(t);
			if (stack.back().isComplete()) stack.pop_back();
			return err;
		}

		switch (t.kind) {
		case Terminal::Kind::PARENTHESIS_OPEN:
			if (empty) {
				empty = false;
			}
			else {
				ParenthesisPair inner;
				stack.push_back(inner);
				return stack.back().parse(t);
			}
			break;
		case Terminal::Kind::PARENTHESIS_CLOSE:
			if (empty) {
				return EXIT_FAILURE;
			}
			else {
				Output::log() << "Pair completed\n";
				complete = true;
			}
			break;
		default: return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	bool isComplete() { return complete; }
private:
	bool empty = true;
	bool complete = false;
	vector<ParenthesisPair> stack;
};

class Program
{
public:
	int parse(Terminal& t)
	{
		Output::log() << Terminal::KIND_NAMES[t.kind] << "\n";

		if (children.empty()) {
			children.push_back(ParenthesisPair{});
		}
		auto err = children.back().parse(t);
		if (children.back().isComplete()) {
			children.pop_back();
		}
		return err;
	}
private:
	vector<ParenthesisPair> children;
};