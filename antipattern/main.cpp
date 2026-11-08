#include <exception>
#include <iostream>
#include <string>
int main(){for(int mode=0;mode<2;++mode){try{if(mode==0)throw 42;throw std::string("primitive string exception");}catch(const std::exception&){std::cout<<"std::exception handler\n";}catch(...){std::cout<<"catch(...) handled non-standard exception "<<mode<<'\n';}}}
