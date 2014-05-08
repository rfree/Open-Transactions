/* See other files here for the LICENCE that applies here. */
/* See header file .hpp for info */

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "utils.hpp"


namespace nOT {
namespace nUtils {

INJECT_OT_COMMON_USING_NAMESPACE_COMMON_1; // <=== namespaces


// text trimming
// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string & ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string & rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
} 

std::string & trim(std::string &s) {
	return ltrim(rtrim(s));
}

cNullstream::cNullstream() { } 

cNullstream g_nullstream; // extern a stream that does nothing (eats/discards data)

// ====================================================================

const char* DbgShortenCodeFileName(const char *s) {
	const char *p = s;
	const char *a = s;
	while (*p) {
		++p;
		if ((*p)=='/') a=p;
	}
	return a;
}


// ====================================================================

std::ostream & cLogger::write_stream(int level) { 
	if ((level >= mLevel) && (mStream)) { *mStream << icon(level) << ' '; return *mStream; } return g_nullstream; 
}

void cLogger::setDebugLevel(int level) {
	bool note_before = (mLevel > level); // report the level change before or after the change? (on higher level)
	if (note_before) _note("Setting debug level to "<<level);
	mLevel = level;
	if (!note_before) _note("Setting debug level to "<<level);
}

cLogger::cLogger() : mStream(NULL), mLevel(20) { mStream = & std::cout; }


std::string cLogger::icon(int level) const {
	if (level >= 100) return "!!!!";
	if (level >=  90) return "!!! ";
	if (level >=  70) return "WARN";
	if (level >=  50) return "Info";
	if (level >=  40) return "Dbg ";
	if (level >=  30) return "dbg ";
	if (level >=  20) return "dbg ";
	return "  ";
}

cLogger current_logger; // extern TODO gCurrentLogger

// ====================================================================
// vector debug

void DisplayStringEndl(std::ostream & out, const std::string text) {
	out << text;
	out << std::endl;
}

std::string cSpaceFromEscape(const std::string &s) {
	std::ostringstream  newStr;
		for(int i = 0; i < s.length();i++) {
			if(s[i] == '\\' && s[i+1] ==32)
				newStr<<"";
			else
				newStr<<s[i];
			}
	return newStr.str();
}


bool CheckIfBegins(const std::string & beggining, const std::string & all) {
	if (all.compare(0, beggining.length(), beggining) == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

std::string cEscapeFromSpace(const std::string &s) { // TODO rename
	std::ostringstream  newStr;
	for(int i = 0; i < s.length();i++) {
		if(s[i] == 32)
			newStr << "\\" << " ";
		else
			newStr << s[i];
	}
	return newStr.str();
}

vector<string> WordsThatMatch(const std::string & sofar, const vector<string> & possib) {
	vector<string> ret;
	for ( auto rec : possib) { // check of possibilities
		if (CheckIfBegins(sofar,rec)) {
			rec = cEscapeFromSpace(rec);
			ret.push_back(rec); // this record matches
		}
	}
	return ret;
}

char GetLastChar(const std::string & str) { // TODO unicode?
	auto s = str.length();
	if (s==0) throw std::runtime_error("Getting last character of empty string (" + ToStr(s) + ")" + OT_CODE_STAMP);
	return str.at( s - 1);
}

std::string GetLastCharIf(const std::string & str) { // TODO unicode?
	auto s = str.length();
	if (s==0) return ""; // empty string signalizes ther is nothing to be returned
	return std::string( 1 , str.at( s - 1) );
}

std::string cEscapeString(const std::string &s) { // TODO rename
	std::ostringstream  newStr;
		for(int i = 0; i < s.length();i++) {
			if(s[i] >=32 && s[i] <= 126)
				newStr<<s[i];
			else
				newStr<<"\\"<< (int) s[i];
			}

	return newStr.str();
}

// ====================================================================

// ASRT - assert. Name like ASSERT() was too long, and ASS() was just... no.
// Use it like this: ASRT( x>y );  with the semicolon at end, a clever trick forces this syntax :)
#define ASRT(x) do { if (!(x)) Assert(false, OT_CODE_STAMP); } while(0)

void Assert(bool result, const std::string &stamp) {
	if (!result) throw std::runtime_error("Assert failed at "+stamp);
}

// ====================================================================
// advanced string

std::string GetMultiline(string endLine) {
	nOT::nUtils::DisplayStringEndl( cout, "Please enter multiple lines of input to be encoded, followed by ~ on a blank line:" );
	std::string result(""); // Taken from OT_CLI_ReadUntilEOF
	while (true) {
		std::string input_line("");
		if (std::getline(std::cin, input_line, '\n'))
		{
			input_line += "\n";
				if (input_line[0] == '~')
					break;
			result += input_line;
		}
		if (std::cin.eof() )
		{
			std::cin.clear();
				break;
		}
		if (std::cin.fail() )
		{
			std::cin.clear();
				break;
		}
		if (std::cin.bad())
		{
			std::cin.clear();
				break;
		}
	}
	return result;
}


}; // namespace nUtil


}; // namespace OT


std::string GetObjectName() {	return GetObjectName_global_string; }
std::string GetObjectName_global_string="(global)"; // extern definition/initialization

