#include <iostream>
#include <cpphtml/CHtml.h>
//g++ -o HtmlTest HtmlTest.cpp -I/usr/local/include -L/usr/local/lib -lcpphtml
//HtmlTest result.html out.html


int main(int argc, char* argv[]) 
{
  if(argc < 2)
  {
	  cout <<" Usage: HtmlTest Infile [outfile]"<<endl;
      return 1;
  }
  char *a1,*a2;
  a1=argv[1];

  if(argc < 3)
  {
	  a2="/dev/stdout";
  }
  else
  {
	  a2=argv[2];
  }

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
	 
