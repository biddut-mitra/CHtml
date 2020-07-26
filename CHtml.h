/*
version 3.28
Part I - Fromat of the HTML
===========================

HTML Data Section Format

1. Data Section Begins With <!-- __CHtml_DATA__  
Note  
	+the __HTML_DATA_BEGIN__ has to be with same line with <!--
	+the data section may appear anywhere in the HTML file but before replacement context


2. Variables in the Data Section has the following format
    __HTML_VARIABLE__::Notes on the variable 
	__NAME_GOES_HERE:: Customer name

3. Data Section ends with __HTML_DATA_END__--> must appear alone
   the __HTML_DATA_END__ may appear on the same line as -->


Example
    <!--  __HTML_DATA_BEGIN__
    __CUSTOMER_NAME__:: Customer name
    __CUSTOMER_ADDRESS__:: Customer address
    __HTML_DATA_END__-->

HTML Body Format
4. Use the variable name #2 anywhere in the HTML file. Must appear exactly as it is described in #2
Example

    Customer: __CUSTOMER_NAME__<br>
	Address : __CUSTOMER_ADDRESS__<br>


ADVANCED
========
Looping/Iteration
------------------

5. To have a repeat block of HTML, can be achieved by the command <-- __HTML_REPEAT_START__ -->.

6. Followed by <-- RepeatBlockId::__HTML_REPEAT_ID__-->. RepeatBlockId will be used to identify the repat block by 
the reat block handler method. 

7. Then the HTML block.

8. The repeat block ends with <!-- __HTML_REPEAT_STOP__--> 



PART II - The handler
=====================

9. Derive a class based on CHtml

   Eg.
      class MySQLHtml: public Chtml
	  {
	      public:
		  void InitiateVars();//must have
		  }

10. Override InitiateVars(). In this method assign all the variables with the replace values.

ADVANCED - For repeat blocks
===========================

11. Override the Repeater(string repeat_id) and call repeater block accordingly.

   Eg.
      class MySQLHtml: public Chtml
      {
	      public:
		  void InitiateVars();//must have even if its empty
		  void Repeater(string repeat_id);//repeat block handler
		  .......
     }

	    


Algorithm
=========
  open html in/out file
  load the data structure
  if data structure empty
     just print the html
     close file
	 return
  endif
  while .not. eof
     readline into s
	 if data structure
	     Populate data store
     endif
  endwhile
  while .not. eof
     readline into s
	 if command 
	    DoCommand(command)
	 else
	    Replace(s)
		Print(s)
	 endif
   endwhile
   close in/out files

*/
#ifndef __CHTML__
#define __CHTML__
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <cctype>//isspace

using namespace std;




#define DO_BLOCK_ID   0
#define DO_DATA_POP   1
#define DO_EXPRESSION 2
#define DO_COMMAND    3
#define DO_REPEAT     4

	typedef 
		std::map <string,string> 
		__replace_variables;
	typedef 
		std::map <string,int> 
		__repeat_locations;


class CHtml
{
    CHtml(const CHtml&);

    CHtml& operator=(const CHtml&);

    ifstream in;
    //typedef vector <string> _rvars;
    //vector <string> _rvars;

	bool ready;
	string why;//why not ready??

	string buffer;//for repeat opration processors
	string htmlfile;
	string outhtml;
public:
    virtual ~CHtml();

    ofstream out;


    __replace_variables  _replace_variables;
	__replace_variables::iterator _repl_var_Iterator;
	__repeat_locations   _repeat_locations;
	__repeat_locations::iterator _rpt_loc_Iterator;

    //CHtml(const char* htmlfile);
	CHtml();//empty contructor Version #2
    CHtml(const string& htmlfile);
    CHtml(const char* htmlfile,const char* outhtml);
	void OpenHtml();
	void Process(int stage=0);
    int ReadDataBlock();
	void Head();//response head
	void Content();//Head calls Content first
	void NewLine();
	//just a oxymoron output
	void Comment(const string &comment);
	//just a oxymoron output when DEBUG is ON
	void Debug(const string& str);

	void Doc();//Head calls Doc after calling Content, cookie should be called before doc
	void Cookie(const string &name,const string &value,const string &fPath="",const string &fComment="",
		const string &fDomain="",  unsigned long fMaxAge=0);

	void PreRepeat();
	void PostRepeat();
    bool IsBottomRepeat();
	//read the next line from input stream
    const istream & NextLine();

    //string operator[](const string& key);
	void Append(string _key,string _value);
    bool IsDataEnds(string opline);
    bool IsDataBegins(string opline);
	void Populate(string opline);
    string RepeaterID(string opline);

    void InitRepeat(string repeat_id);

    void Replace(string& modifyMe,   string findMe, string newChars);
	//does the string has "__HTML_REPEAT_START__"
    bool IsRepeat(string opline);
    bool IsEndRepeat(string opline);

	//inline department
	inline bool IsReady() { return ready;}
	inline const string& WhyNot() { return why;}//return why not ready
	inline string ltrim(string const &str) 
	{
		for (unsigned i = 0; i < str.size(); i++)
			if (!isspace(str[i]))
				return str.substr(i);
		return "";
	}

	inline string rtrim(string const &str) 
	{
		for (int i = str.size() - 1; i >= 0; i--)
			if (!isspace(str[i]))
				return str.substr(0, i + 1);
		return "";
	}

	inline string trim(string const &str) 
	{
		return rtrim(ltrim(str));
	}
	//virtualaeeeeee
    virtual void InitiateVars();
    virtual void Repeater(const string& repeat_id);//replaces repeatedly
	virtual void Replacer(string& op_buffer,   string html_id, string replace_val);//replaces each item, it is called each
	//time it finds a html_id. You can ignore the replace_val.
	//InitiateVars is called once to replace all vaiables, it takes memory. Replacer does take memory,but it takes 
	//process time. 

	//Setting infile Version #2
	void SetFileNames(const string &in_html_file_name,const string &out_html_file_name);
	//Added on Mar 12 2009
	void Output(const string& output_str);

};

#endif //of __CHTML__

