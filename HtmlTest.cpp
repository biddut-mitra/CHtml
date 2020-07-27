#include <iostream>
#include <CHtml.h>
//g++ -o HtmlTest     CHtml.cpp HtmlTest.cpp -I .
//or
//g++ -o HtmlTest.cgi CHtml.cpp HtmlTest.cpp -I .

//testing
// ./HtmlTest.cgi result.html /dev/stdout
// ./HtmlTest.cgi result.html index.html


int main(int argc, char* argv[]) 
{
  if(argc < 3)
  {
	  cout <<" Usage: HtmlTest infile outfile"<<endl;
      return 1;
  }
  char *a1,*a2;
  a1=argv[1];

  //if(argc < 3)
  //{
  //	  a2=const_cast<char*> ( string("/dev/stdout").c_str());
  //}
  //else
  //{
	  a2=argv[2];
  //}

  CHtml h(a1,a2);
  if(h.IsReady())
  {
	  int stage=h.ReadDataBlock();
	  h.Append("__result_header__","Example header");
	  h.Append("__result_body__","this is the body of the messsage.");
	  h.Append("__author__","Jahan");
	  h.Append("__source__",__FILE__);
	  h.Process(stage);
  }
  else
	  cout <<" CHtml is not ready to process."<<endl;

}
	 
