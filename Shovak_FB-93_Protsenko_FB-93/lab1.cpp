#include <iostream>
#include <vector>
#include <regex>
using namespace std;

struct TokenT
{
	string name, reg;

	TokenT()
	{
		this->name = "";
		this->reg = "";
	}

	TokenT(string name, string reg)
	{
		this->name = name;
		this->reg = reg;
	}
};

TokenT List[] = {
	{"CREATE", "CREATE"},
	{"INDEXED", "INDEXED"},
	{"INSERT", "INSERT( INTO)?"},
	{"SELECT", "SELECT"},
	{"DELETE", "DELETE( FROM)?"},
	{"FROM", "FROM"},
	{"WHERE", "WHERE"},
	{"JOIN", "JOIN"},
	{"ON", "ON"},
	{"SPACE", "[ \\t\\n\\r\\v\\f]"},
	{"NAME", "[a-z_]{1,}"},
	{"VALUE", "\\`[a-zA-Z0-9_]{1,}\\`"},
	{"Left_BRACKET", "\\("},
	{"Right_BRACKET", "\\)"},
	{"COMMA", ","},
	{"ALL", "\\*"},
	{"EQUALS", "="},
	{"NOT_EQUALS", "!="},
	{"GREATER_THAN_EQUALS", ">="},
	{"LESS_THAN_EQUALS", "<="},
	{"GREATER", ">"},
	{"LESS", "<"},
	{"SEMICOLON", ";"},
};

const int ListSize = sizeof(List) / sizeof(*List);

string TokenTRegex(string character)
{
	for (int i = 0; i < ListSize; i++)
	{
		if (List[i].name == character)
		{
			return List[i].reg;
		}
	}

	cout << "This character does not exist!:)" << endl;

	throw;
}

struct Token
{
	TokenT character;
	string content;
	int pos;

	Token(TokenT& character, string content, int pos)
	{
		this->character = character;
		this->content = content;
		this->pos = pos;
	}

	void print()
	{
		cout << this->character.name << "\t\t\t" << this->content << "\t\t\t\t" << this->pos << endl;
	}
};

class Lexer
{
private:
	string input;
	int pos;
	vector<Token> TokenL;

public:
	Lexer(string input)
	{
		this->input = input;
		pos = 0;
	}

	vector<Token> review()
	{
		while (this->pos < this->input.length())
		{
			bool audit = true;

			string input = this->input.substr(this->pos);
			smatch match;

			for (int i = 0; i < ListSize; i++)
			{
				regex regular("^" + List[i].reg);

				if (regex_search(input, match, regular))
				{
					if (List[i].name != "SPACE")
					{
						Token token(List[i], match[0], this->pos);
						this->TokenL.push_back(token);
					}

					this->pos += match[0].length();
					audit = false;
					break;
				}
			}

			if (audit)
			{
				cout << "Mistake on position!:)" << this->pos << endl;
				throw;
			}
		}
		return this->TokenL;
	}
};

int main()
{
	string input;
	cout << "Input commands>>> \n\n";
	getline(cin, input, ';');	

	Lexer lexer(input);

	vector<Token> TList = lexer.review();
	
	cout << "Character>>> " << "\t\tContent>>> " << "\t\tPosition>>> " << endl << endl;
	for (int i = 0; i < TList.size(); i++)
	{
		TList[i].print();
	}
	
	return 0;
}