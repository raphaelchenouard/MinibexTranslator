//
//  Created by Raphaël Chenouard on 13/02/2020.
//

#ifndef IBEX_EXPR_GAMS_H
#define IBEX_EXPR_GAMS_H

#include "ibex.h"

using namespace ibex;
using namespace std;

class Expr2GAMS : public ExprPrinter{
protected:
    void visit(const ExprIndex& e) {
        e.expr.acceptVisitor(*this);
        (*os) <<"('";
        const DoubleIndex &idx = e.index;

        switch (idx.dim.type()) {
        case Dim::ROW_VECTOR:
            if (idx.one_col()) (*os) << idx.first_col();
            else (*os) << idx.first_col() << ":" << idx.last_col();
            break;
        case Dim::COL_VECTOR:
            if (idx.one_row()) (*os) << idx.first_row();
            else (*os) << idx.first_row() << ":" << idx.last_row();
            break;
        default:
            assert(idx.dim.is_matrix());
            if (idx.all_rows()) (*os) << ":";
            else if (idx.one_row()) (*os) << idx.first_row();
            else (*os) << idx.first_row() << ":" << idx.last_row();
            (*os) << ",";
            if (idx.all_cols()) (*os) << ":";
            else if (idx.one_col()) (*os) << idx.first_col();
            else (*os) << idx.first_col() << ":" << idx.last_col();
            
        }
        (*os)<<"')";
    }
};

#endif // end IBEX_EXPR_GAMS_H
