//
//  Minibex2AMPL.cpp
//  
//
//  Created by Raphaël Chenouard on 26/02/2020.
//

#include "Minibex2AMPL.h"

#define DEBUG 1

bool system2ampl(const System& sys, const char* fname){
    string ampl_str = "# ";
    ampl_str+=fname;
    ampl_str+="\n\n# Variables with bounds:\n";
    // Writing variables
    for(int i=0;i<sys.args.size();i++){
        string ampl_var = "var ";
        ampl_var += sys.args[i].name;
        if (sys.args[i].type()==Dim::MATRIX){
            ampl_var += "{i in 0..";
            ampl_var += to_string(sys.args[i].dim.nb_rows()-1);
            ampl_var += ",j in 0..";
            ampl_var += to_string(sys.args[i].dim.nb_cols()-1);
            ampl_var += "}";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            ampl_var += "{i in 0..";
            ampl_var += to_string(sys.args[i].dim.nb_cols()-1);
            ampl_var += "}";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            ampl_var += "{j in 0..";
            ampl_var += to_string(sys.args[i].dim.nb_rows()-1);
            ampl_var += "}";
        }
        ampl_var += " >= ";
        ampl_var += to_string(sys.box[i].lb());
        ampl_var += " <= ";
        ampl_var += to_string(sys.box[i].ub());
        ampl_var += ";\n";
        ampl_str += ampl_var;
    }
    Expr2AMPL prt;
    // Writing objective function
    if(sys.goal != NULL){
        ampl_str += "\n\n# Objective to minimize:\nminimize obj: ";
        ostringstream fobj;
        //fobj << sys.goal->expr();
        prt.print(fobj,sys.goal->expr());
        ampl_str += fobj.str();
        ampl_str += ";\n";
    }
    // Writing constraints
    ampl_str+="\n\n# Constraints:\n";
    for(int i=0;i<sys.ctrs.size();i++){
        string ampl_ctr = "subject to c"+to_string(i)+":\n\t";
        ostringstream stream;
        //stream << sys.ctrs[i];
        prt.print(stream,sys.ctrs[i].f.expr());
        stream<<sys.ctrs[i].op;
        if (sys.ctrs[i].op==LT || sys.ctrs[i].op==LEQ){
            stream<<sys.ctrs[i].right_hand_side().i().ub();
        }
        else{
            stream<<sys.ctrs[i].right_hand_side().i().lb();
        }
        ampl_ctr += stream.str()+";\n";
        ampl_str += ampl_ctr;
    }
    
    ampl_str += "\n\n# Starting point (mid of initial box):\n";
    for(int i=0; i<sys.args.size();i++){
        ampl_str+= "let ";
        if (sys.args[i].type()==Dim::MATRIX){
            ampl_str += "{i in 0..";
            ampl_str += to_string(sys.args[i].dim.nb_rows()-1);
            ampl_str += ",j in 0..";
            ampl_str += to_string(sys.args[i].dim.nb_cols()-1);
            ampl_str += "} ";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            ampl_str += "{i in 0..";
            ampl_str += to_string(sys.args[i].dim.nb_cols()-1);
            ampl_str += "} ";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            ampl_str += "{j in 0..";
            ampl_str += to_string(sys.args[i].dim.nb_rows()-1);
            ampl_str += "} ";
        }
        
        ampl_str+=sys.args[i].name;
        
        if (sys.args[i].type()==Dim::MATRIX){
            ampl_str += "[i,j]";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            ampl_str += "[i]";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            ampl_str += "[j]";
        }
        
        ampl_str+= " := ";
        ampl_str+= to_string(sys.box[i].mid());
        ampl_str+= ";\n";
    }
    
#ifdef DEBUG
    cout<<ampl_str<<endl;
    for(int i=0;i<sys.var_names().size();i++)
        cout<<sys.var_names()[i]<<" ";
    cout<<endl;
#endif
    ofstream mod_file;
    mod_file.open(fname);
    mod_file<<ampl_str;
    mod_file.close();
    cout<<fname<<" written"<<endl;
    return true;
}

int main(int argc, char** argv){
    char *fname;
    if (argc==2){
        fname=argv[1];
    }
    else {
        cout<<"No file name provided"<<endl;
        exit(1);
    }
    
    string ampl_fname=fname;
    ampl_fname+=".mod";
    System s(fname);
    if(system2ampl(s,ampl_fname.c_str())){
        cout<<"Success!"<<endl;
    }
    else{
        cout<<"FAIL!"<<endl;
    }
    
    return 0;
}
