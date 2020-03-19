
#include "Expr2AMPL.h"


int main(int argc, char** argv){
    Expr2AMPL printer;
    
    Variable x("x");
    Function f1(x,x+2);
    printer.print(cout,f1.expr());
    cout<<endl;
    
    Variable y(2,"y");
    Function f2(y,y[0]+y[1]);
    printer.print(cout,f2.expr());
    cout<<endl;

    Variable z(2,2,"z");
    Function f3(z,z[0][1]+z[1][0]);
    //Function f3("z","z(1)(2)+z(2)(1)");
    printer.print(cout,f3.expr());
    cout<<endl;
    
    System sys1("test_mat.mbx");
    printer.print(cout,sys1.goal->expr());
    cout<<endl<<endl<<sys1<<endl;
    
    System sys2("test_mat2.mbx");
    printer.print(cout,sys2.goal->expr());
    cout<<endl<<endl<<sys2<<endl;

    return 0;
}
