/*
ver 3.28
Part I - Fromat of the HTML
===========================

HTML Data Section Format

1. Data Section Begins With <!-- 
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
4. Use the variable names declared in step #2 anywhere in the HTML file. 
Must appear exactly as it is described in #2
Example

    Customer: __CUSTOMER_NAME__<br>
	Address : __CUSTOMER_ADDRESS__<br>


ADVANCED
========
Looping/Iteration
------------------

5. To have a repeat block of HTML can be achieved by the command 
<!-- __HTML_REPEAT_START__ -->.

6. Followed by <!-- RepeatBlockId1::__HTML_REPEAT_ID__-->. 
RepeatBlockId will be used to identify the repat block by 
the repeat block handler method described below in the PART II. 

7. Then the HTML block.

8. The repeat block ends with <!-- __HTML_REPEAT_STOP__ --> 



PART II - The handler
=====================

9. Derive a class based on CHtml

   Eg.
      class MySQLHtml: public Chtml
	  {
	      public:
		  void InitiateVars();//override base virtual method
		  }

10. Override InitiateVars(). In this method assign all the variables with the default replace values.
Example.
        
         Append("__Author__","Aftab Jahan Subedar");

    

ADVANCED - For repeat blocks
===========================

11. Override the Repeater(string repeat_id) and call repeater block accordingly.

   Eg.
      class MySQLHtml: public CHtml
	  {
	      public:
		  void InitiateVars();//must have even if its empty
		  void Repeater(string repeat_id);//repeat handler
		  }

  	if(repeat_id==string("Explorer"))
	{ 
		  //list the files and directories
		  Comment("We are in explorer, processing repeat_id");

		  //minimum requirement for Repeater method
		  bool NotBottom=true;

		  InitRepeat(repeat_id);//just store the repeat location for this loopid
		  while(NotBottom && NextLine()) 
		  {
				 PreRepeat();//Check if its a repeat command
				 if(IsBottomRepeat())//we have reached the bottom of HTML_REPEAT_STOP
				 {

					 NotBottom=false;
				 }
				 PostRepeat();//replace any variable in buffer if we find any
		  }

	 	  return;
	 }

	if(repeat_id==string("Loginfo"))
	 {
		//sql_query="SELECT DATE_FORMAT(LoggedFrom,'%a %b %D %Y %H:%M:S'),LoggedFrom  FROM WebLogging";
		sql_query="SELECT LoggedFrom  FROM WebLogging";
		//sql_query+=" WHERE User='"+strWhereUser+"'";
		//sql_query+=" WHERE User='"+strWhereUser+"' ORDER BY LastProcessedOn DESC";
		sql_query+=" WHERE EMail='"+strWhereUser+"' AND MONTH(NOW())=MONTH(LastProcessedOn) AND YEAR(NOW())=YEAR(LastProcessedOn) ORDER BY LastProcessedOn DESC";
		//out <<"<!--"<<str.c_str()<<"-->"<<endl;

		out << "<!-- SQL: "<< sql_query.c_str()<<" -->"<<endl;
		out << "<!-- RepeatID: "<< repeat_id.c_str()<<" -->"<<endl;
		PrintRecords(repeat_id.c_str());
		return;
	 }
	    
12. Calling/Using

  i.  instantiate an object of type CHtml with constructor x("result.html") marker container
  ii. int stage=ReadDatablock
  iii. Call DocTypeOut () if required
  iv.call x.Append(xxx)
  v. Process(stage)
  Walla




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
/*
Version 3.00
*/

#include "CHtml.h"

CHtml::CHtml()//empty contructor Version #2
{
	ready=false;
}
CHtml::CHtml(const CHtml& f)
{
	if(f.ready)
	{
		  ready=f.ready;
		  htmlfile=f.htmlfile;
		  OpenHtml();

	}
}
CHtml& CHtml::operator=(const CHtml& f)
{
	if(f.ready)
	{
		  ready=f.ready;
		  htmlfile=f.htmlfile;
		  OpenHtml();

	}
	return *this;
}
//Version #2 ends here

//CHtml::CHtml(const char* htmlfile)
CHtml::CHtml(const string& htmlfile_arg)
{
  htmlfile=htmlfile_arg;
  OpenHtml();
}
//internal call
void CHtml::OpenHtml()
{
  ready=true;
  in.open(htmlfile.c_str());
  if(!in)
  {
	   why="Input file open failed";
	   ready=false;
  }

  out.open("/dev/stdout");
  if(!out)
  {
	   why="Output file open failed";
	   ready=false;
  }
}

void CHtml::SetFileNames(const string &in_html_file_name,const string &out_html_file_name)
{
  ready=true;
  in.open(in_html_file_name.c_str());
  if(!in)
  {
	   why="Input file open failed";
	   ready=false;
  }

 if(out_html_file_name.empty())
	 out.open("/dev/stdout");
 else
     out.open(out_html_file_name.c_str());

  if(!out)
  {
	   why="Output file open failed";
	   ready=false;
  }
}

CHtml::~CHtml()
{
}
CHtml::CHtml(const char* htmlfile,const char* outhtml)
{
  ready=true;

  in.open(htmlfile);
  if(!in)
  {
	   why="Input file open failed";
	   ready=false;
  }

  out.open(outhtml);
  if(!out)
  {
	   why="Output file open failed";
	   ready=false;
  }

}
void CHtml::Content()
{
	out << "Content-type: text/html"<<endl;
}
void CHtml::NewLine()
{
	 out << endl;
}

void CHtml::Comment(const string& comment)
{
	 out <<"<!-- "<<comment.c_str()<<" -->"<<endl;
}
void CHtml::Doc()
{
	 out << endl;
     //out <<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"<<endl;
	 out <<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"  \"http://www.w3.org/TR/html4/strict.dtd\">"<<endl;

}
void CHtml::Cookie(const string &name,const string &value,const string &fPath,
				   const string &fComment,const string &fDomain, unsigned long fMaxAge)
{

 out << "Set-Cookie: " << name.c_str() << '=' << value.c_str();
  if(fComment.empty() == false)
    out << "; Comment=" << fComment;
  if(fDomain.empty() == false)
    out << "; Domain=" << fDomain;
  if(fMaxAge != 0)
    out << "; Max-Age=" << fMaxAge;
  if(fPath.empty() == false)
    out << "; Path=" << fPath;
  //if(fSecure == true)
   // out << "; Secure";
  
  out << "; Version=1"<<endl;//\r\n\r
}
//first call 
void CHtml::Head()
{
	//call this if you do not have any doc type set in your html file
	 Content();
	 Doc();

}

void  CHtml::Repeater(const string& repeat_id)
{
}
void  CHtml::Replacer(string& op_buffer,   string html_var, string replace_val)
{
  Replace(op_buffer,html_var,replace_val);

}
void CHtml::InitiateVars()
{
  Append("__Author__","Aftab Jahan Subedar");
}

void CHtml::PreRepeat()
{
		if(IsRepeat(buffer))//__HTML_REPEAT_START__
		{
			//out << "<!-- calling repeater -->"<< endl;
			if(NextLine())
				Repeater(RepeaterID(buffer));
			//out << "<!-- called repeater -->"<< endl;

		}
}

bool CHtml::IsBottomRepeat()
{
		if(IsEndRepeat(buffer))//__HTML_REPEAT_STOP__
		{
			buffer=string();
			//Debug("buffer clean");
			return true;
		}
		return false;
		//InitRepeat(repeat_id);//go start from the begining of repeat start
}

void CHtml::PostRepeat()
{
	    if(buffer.empty())
			return;
		for(_repl_var_Iterator=_replace_variables.begin();_repl_var_Iterator!=_replace_variables.end();_repl_var_Iterator++)
		{
			if(buffer.find((*_repl_var_Iterator).first) !=string::npos)
				Replacer(buffer,(*_repl_var_Iterator).first,(*_repl_var_Iterator).second);


		}
		out << buffer.c_str()<<endl;
}

//call append as much as u can after u call read data block and
//before process
void CHtml::Append(string _key,string _val)
{
	_replace_variables[_key]=_val;
   //out << "<!-- Appending Key :"<<_key.c_str()<<" Value:"<<_val.c_str()<<"-->"<<endl;
}

void  CHtml::Replace(string& modifyMe,   string findMe, string newChars)
{
  // Look in modifyMe for the "find string"
  // starting at position 0
  //unsigned int i = modifyMe.find(findMe, 0);
  std::size_t i = modifyMe.find(findMe, 0);	
  // Did we find the string to replace?
  if(i != string::npos)
    // Replace the find string with newChars
    modifyMe.replace(i,findMe.size(),newChars);
}

bool CHtml::IsDataBegins(string opline)
{
	//if(opline.find("<!--") != string::npos) 
	if(opline.find("__HTML_DATA_BEGIN__") != string::npos) 
		{
             return true;
		}
	return false;
}
bool CHtml::IsDataEnds(string opline)
{
	if(opline.find("__HTML_DATA_END__") != string::npos) 
	//if(opline.find("-->") != string::npos) 
		{
             return true;
		}
	return false;
}

bool CHtml::IsRepeat(string opline)
{
	if(opline.find("__HTML_REPEAT_START__") != string::npos) 
	//if((beginsAt=opline.find("-->")) != string::npos) 
		{
             return true;
		}
	return false;
}
bool CHtml::IsEndRepeat(string opline)
{
	if(opline.find("__HTML_REPEAT_STOP__") != string::npos) 
	//if((beginsAt=opline.find("-->")) != string::npos) 
		{
             return true;
		}
	return false;
}
string CHtml::RepeaterID(string opline)
{
       if(opline.find("__HTML_REPEAT_ID__") !=string::npos) 
	   {
				opline = opline.substr(opline.find_first_of("<!--")+string("<!--").size());
				string var=trim(opline.substr(0,opline.find("::") ));  
				if(!var.empty())
				 return var;

	   }
		return string();
}
int  CHtml::ReadDataBlock()
{
	//now you have to call the DocTypeOut() by the caller first

  //out << "Content-type: text/html\n\n"<<endl;
//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
//<html lang="en" dir="ltr" >  

  string line,opline,holder;
  int stage=DO_BLOCK_ID;
  //we are looking for BLOCK_ID, can be anywhere before its usage
  while(getline(in, line) && stage==DO_BLOCK_ID) 
  {
	     opline=line;
		 if(IsDataBegins(opline))//we found a block
		 {
			 out <<"<!-- Subedar Technologies http://www.SubedarTechnologies.com -->"<<endl;
			 out <<"<!-- Subedar Technologies, Simply Technologies -->"<<endl;
			 stage=DO_DATA_POP;//at next pass
			 opline=string();//eat the current string
		 }
		 else
		 {
			out << opline.c_str()<<endl;
			opline=string();
		 }
  }
  //if DO_DATA_POP is set,cuz we found a BLOCK_ID section
  while( stage==DO_DATA_POP && getline(in, line)) 
  {
	     opline=line;

		if(IsDataEnds(opline))
		{
			opline=string();//eat the current string
			stage=DO_COMMAND;//commands 

		}
		else
		{
			Populate(opline);
			opline=string();//eat the current string
		}
	}
    //out << "<!-- last line from ReadDataBlock "<<line<<"-->"<<endl;

   return stage;
}

//called with the value returned by ReadDataBlock
//and after the InitiateVars or Append
//stage=ReadDataBlock();

void CHtml::Process(int stage)
{
  string line,opline,holder;

  InitiateVars();//February 9th 2004

  while( stage==DO_COMMAND && getline(in, line)) 
  {
        //out << "<!-- line from Process "<<line<<"-->"<<endl;
	    opline=line;
		if(IsRepeat(opline))//__HTML_REPEAT_START__
		{
			if(getline(in, opline))
				Repeater(RepeaterID(opline));
			opline=string();

			//stay on the same stage
		}
		else
		{
			for(_repl_var_Iterator=_replace_variables.begin();_repl_var_Iterator!=_replace_variables.end();_repl_var_Iterator++)
			{
				if(opline.find((*_repl_var_Iterator).first) !=string::npos)
					Replacer(opline,(*_repl_var_Iterator).first,(*_repl_var_Iterator).second);
			}
			out << opline.c_str()<<endl;
			opline=string();
		}
	}

  in.close();
  out.close();

}
void CHtml::Populate(string opline)
{
        string var=trim(opline.substr(0,opline.find("::") ));  
		if(!var.empty())
		   Append(var,var);//append can be called later or before the called to poputlate
}



void CHtml::InitRepeat(string repeat_id)
{
            _rpt_loc_Iterator = _repeat_locations.find(repeat_id);
            if(_rpt_loc_Iterator != _repeat_locations.end() )    
			{//found
				in.seekg(_repeat_locations[repeat_id]);//go start from the begining of repeat start
                //cout << (*_repeat_locations_Iterator).second << " ";
			}
			else
			{//not found
				_repeat_locations[repeat_id]=in.tellg();

			}

}
const istream & CHtml::NextLine()
{
	return getline(in, buffer);
}
void CHtml::Debug(const string& str)
{
#ifdef __DEVEL__
				out <<"<!--"<<str.c_str()<<"-->"<<endl;
#endif


}

