//
//  Minibex2GAMS.cpp
//  
//
//  Created by Raphaël Chenouard on 26/02/2020.
//

#include "Minibex2GAMS.h"

#define DEBUG 1

int* indexed_symbol (const System& sys){
    int counter=0;
    int* idx = (int*)malloc(sys.args.size()*sizeof(int));
    for(int i=0;i<sys.args.size();i++){
        if (sys.args[i].type()!=Dim::SCALAR){
            counter++;
            idx[i]=counter;
        }
        else{
            idx[i]=0;
        }
    }
    if (counter==0){
        free(idx);
        return NULL;
    }
    else{
        return idx;
    }
}

bool system2gams(const System& sys, const char* fname){
    string gams_str = "* ";
    gams_str+=fname;
    
    int* indexed = indexed_symbol(sys);
    
    if (indexed!=NULL){
        gams_str+="\n\n* Sets and parameters:\n\nSet\n";
        for(int i=0;i<sys.args.size();i++){
            if (sys.args[i].type()==Dim::MATRIX){
                gams_str += "\tidx";
                gams_str += to_string(indexed[i]);
                gams_str += " / 0*";
                gams_str += to_string(sys.args[i].dim.nb_rows()-1);
                gams_str += " / \n";
                
                gams_str += "\tjdx";
                gams_str += to_string(indexed[i]);
                gams_str += " / 0*";
                gams_str += to_string(sys.args[i].dim.nb_cols()-1);
                gams_str += " / \n";
            }
            else if (sys.args[i].type()==Dim::ROW_VECTOR){
                gams_str += "\tjdx";
                gams_str += to_string(indexed[i]);
                gams_str += " / 0*";
                gams_str += to_string(sys.args[i].dim.nb_cols()-1);
                gams_str += " / \n";
            }
            else if (sys.args[i].type()==Dim::COL_VECTOR){
                gams_str += "\tidx";
                gams_str += to_string(indexed[i]);
                gams_str += " / 0*";
                gams_str += to_string(sys.args[i].dim.nb_rows()-1);
                gams_str += " / \n";
            }
        }
        gams_str += "\t;\n";
    }
    gams_str+="\n\n* Variables:\nVariable\n";
    // Writing variables
    for(int i=0;i<sys.args.size();i++){
        string gams_var = "\t";
        gams_var += sys.args[i].name;
        if (sys.args[i].type()==Dim::MATRIX){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ",jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")\n";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            gams_var += "jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")\n";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ")\n";
        }
        gams_str += gams_var;
    }
    gams_str += "\tobj;\n";
    Expr2GAMS prt;
    // Writing objective function
    if(sys.goal != NULL){
        gams_str += "\n\n* Objective to minimize:\nEquation \n\tfobj 'Objective function';\nfobj.. obj =e= ";
        ostringstream fobj;
        prt.print(fobj,sys.goal->expr());
        gams_str += fobj.str();
        gams_str += ";\n";
    }
    // Writing constraints
    if(sys.ctrs.size()>0){
        gams_str+="\n\n* Constraints:\nEquation\n";
        for(int i=0;i<sys.ctrs.size();i++){
            gams_str += "\tc" + to_string(i) + "\n";
        }
        gams_str += "\t;\n";
        for(int i=0;i<sys.ctrs.size();i++){
            string gams_ctr = "c"+to_string(i)+"..\t";
            ostringstream stream;
            //stream << sys.ctrs[i];
            prt.print(stream,sys.ctrs[i].f.expr());
            if (sys.ctrs[i].op==LT || sys.ctrs[i].op==LEQ){
                stream<<" =l= "<<sys.ctrs[i].right_hand_side().i().ub();
            }
            else if (sys.ctrs[i].op==GT || sys.ctrs[i].op==GEQ){
                stream<<" =g= "<<sys.ctrs[i].right_hand_side().i().lb();
            }
            else { // EQ
                stream<<" =e= "<<sys.ctrs[i].right_hand_side().i().lb();
            }
            gams_ctr += stream.str()+";\n";
            gams_str += gams_ctr;
        }
    }
    
    gams_str+="\n\n* Bounds for Variables:\n";
    // Writing variables
    for(int i=0;i<sys.args.size();i++){
        string gams_var = "";
        gams_var += sys.args[i].name;
        gams_var += ".lo";
        if (sys.args[i].type()==Dim::MATRIX){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ",jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            gams_var += "jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        gams_var += " = ";
        gams_var += to_string(sys.box[i].lb());
        gams_var += ";\n";
        gams_var += sys.args[i].name;
        gams_var += ".up";
        if (sys.args[i].type()==Dim::MATRIX){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ",jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            gams_var += "jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        gams_var += " = ";
        gams_var += to_string(sys.box[i].ub());
        gams_var += ";\n";
        gams_str += gams_var;
    }
    
    gams_str += "\n\n* Starting point (mid of initial box):\n";
    for(int i=0; i<sys.args.size();i++){
        string gams_var = "";
        gams_var += sys.args[i].name;
        gams_var += ".l";
        if (sys.args[i].type()==Dim::MATRIX){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ",jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        else if (sys.args[i].type()==Dim::ROW_VECTOR){
            gams_var += "jdx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        else if (sys.args[i].type()==Dim::COL_VECTOR){
            gams_var += "(idx";
            gams_var += to_string(indexed[i]);
            gams_var += ")";
        }
        gams_var += " = ";
        gams_var += to_string(sys.box[i].mid());
        gams_var += ";\n";
        gams_str += gams_var;
    }
    
    gams_str += "\n\nModel Mbx / all / ;\n\n";
    gams_str += "solve mbx minimizing obj using nlp;\n";
    
    
#ifdef DEBUG
    cout<<gams_str<<endl;
    for(int i=0;i<sys.var_names().size();i++)
        cout<<sys.var_names()[i]<<" ";
    cout<<endl;
#endif
    ofstream gms_file;
    gms_file.open(fname);
    gms_file<<gams_str;
    gms_file.close();
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
    
    string gams_fname=fname;
    gams_fname+=".gms";
    System s(fname);
    if(system2gams(s,gams_fname.c_str())){
        cout<<"Success!"<<endl;
    }
    else{
        cout<<"FAIL!"<<endl;
    }
    
    return 0;
}
