DEF_FUNC(SIN, sin, sin, {Knot* mul_knot = knot_op_creater (PKNOT, dR, KNOT_CLONE, MUL); \
                        R(mul_knot)->function = COS;                                    \
                        KNOT = mul_knot;})

DEF_FUNC(COS, cos, cos, {Knot* mul_knot  = knot_op_creater (PKNOT, dR, nullptr, MUL);           \
                        R(mul_knot) = knot_op_creater (mul_knot, nullptr, KNOT_CLONE, MUL);     \
                        RL(mul_knot) = knot_num_creater (R(mul_knot), nullptr, nullptr, -1);    \
                        RR(mul_knot)->function = SIN;                                           \
                        KNOT = mul_knot;})

DEF_FUNC(TG,  tan, tan, {Knot* div_knot = knot_op_creater (PKNOT, dR, nullptr, DIV);            \
                        R(div_knot) = knot_op_creater (div_knot, KNOT_CLONE, nullptr, POW);     \
                        RR(div_knot) = knot_num_creater (R(div_knot), nullptr, nullptr, 2);     \
                        RL(div_knot)->function = COS;                                           \
                        KNOT = div_knot;})

DEF_FUNC(LN, log, ln,   {TYPE_KNOT = OPERATION; \
                        OP_KNOT    = DIV;       \
                        LKNOT      = dR;})
            
DEF_FUNC(ARCSIN, asin, arcsin,  {Knot* div_knot = knot_op_creater (PKNOT, dR, nullptr, DIV);                    \
                                R(div_knot) = knot_func_creater (div_knot, nullptr, nullptr, SQRT);             \
                                RR(div_knot) = knot_op_creater (R(div_knot), nullptr, nullptr, SUB);            \
                                RRL(div_knot) = knot_num_creater (div_knot->right->right, nullptr, nullptr, 1); \
                                RRR(div_knot) = knot_op_creater (RR(div_knot), RKNOT_CLONE, nullptr, POW);      \
                                RRRR(div_knot) = knot_num_creater (RRR(div_knot), nullptr, nullptr, 2);         \
                                KNOT = div_knot;})          

DEF_FUNC(ARCCOS, acos, arccos,  {Knot* div_knot = knot_op_creater (PKNOT, nullptr, nullptr, DIV);           \
                                L(div_knot) = knot_op_creater (div_knot, nullptr, dR, MUL);                 \
                                LL(div_knot) = knot_num_creater (L(div_knot), nullptr, nullptr, -1);        \
                                R(div_knot) = knot_func_creater (div_knot, nullptr, nullptr, SQRT);         \
                                RR(div_knot) = knot_op_creater (R(div_knot), nullptr, nullptr, SUB);        \
                                RRL(div_knot) = knot_num_creater (RR(div_knot), nullptr, nullptr, 1);       \
                                RRR(div_knot) = knot_op_creater (RR(div_knot), RKNOT_CLONE, nullptr, POW);  \
                                RRRR(div_knot) = knot_num_creater (RRR(div_knot), nullptr, nullptr, 2);     \
                                KNOT = div_knot;})

DEF_FUNC(ARCTG,  atan, arctan,  {Knot* div_knot = knot_op_creater (current_knot->prev, dR, nullptr, DIV);   \
                                R(div_knot) = knot_op_creater (div_knot, nullptr, nullptr, ADD);            \
                                RL(div_knot) = knot_num_creater (R(div_knot), nullptr, nullptr, 1);         \
                                RR(div_knot) = knot_op_creater (R(div_knot), RKNOT_CLONE, nullptr, POW);    \
                                RRR(div_knot) = knot_num_creater (RR(div_knot), nullptr, nullptr, 2);       \
                                KNOT = div_knot;})

DEF_FUNC(SH, sinh, sinh,    {Knot* mul_knot = knot_op_creater(PKNOT, dR, KNOT_CLONE, MUL);  \
                            R(mul_knot)->function = CH;                                     \
                            KNOT = mul_knot;})  

DEF_FUNC(CH, cosh, cosh,    {Knot* mul_knot = knot_op_creater(PKNOT, dR, KNOT_CLONE, MUL);  \
                            R(mul_knot)->function = SH;                                     \
                            KNOT = mul_knot;})  

DEF_FUNC(TH, tanh, tanh,    {Knot* div_knot = knot_op_creater(PKNOT, dR, nullptr, DIV);         \
                            R(div_knot) = knot_op_creater(div_knot, KNOT_CLONE, nullptr, POW);  \
                            RR(div_knot) = knot_num_creater (R(div_knot), nullptr, nullptr, 2); \
                            RL(div_knot)->function = CH;                                        \
                            KNOT = div_knot;})

DEF_FUNC(ARCSH, asinh, arcsinh  {Knot* div_knot = knot_op_creater (PKNOT, dR, nullptr, DIV);                    \
                                R(div_knot) = knot_func_creater (div_knot, nullptr, nullptr, SQRT);             \
                                RR(div_knot) = knot_op_creater (R(div_knot), nullptr, nullptr, ADD);            \
                                RRL(div_knot) = knot_num_creater (div_knot->right->right, nullptr, nullptr, 1); \
                                RRR(div_knot) = knot_op_creater (RR(div_knot), RKNOT_CLONE, nullptr, POW);      \
                                RRRR(div_knot) = knot_num_creater (RRR(div_knot), nullptr, nullptr, 2);         \
                                KNOT = div_knot;})

DEF_FUNC(ARCCH, acosh, arccosh, {Knot* div_knot = knot_op_creater (PKNOT, nullptr, nullptr, DIV);           \
                                L(div_knot) = knot_op_creater (div_knot, nullptr, dR, MUL);                 \
                                LL(div_knot) = knot_num_creater (L(div_knot), nullptr, nullptr, -1);        \
                                R(div_knot) = knot_func_creater (div_knot, nullptr, nullptr, SQRT);         \
                                RR(div_knot) = knot_op_creater (R(div_knot), nullptr, nullptr, ADD);        \
                                RRL(div_knot) = knot_num_creater (RR(div_knot), nullptr, nullptr, 1);       \
                                RRR(div_knot) = knot_op_creater (RR(div_knot), RKNOT_CLONE, nullptr, POW);  \
                                RRRR(div_knot) = knot_num_creater (RRR(div_knot), nullptr, nullptr, 2);     \
                                KNOT = div_knot;})

DEF_FUNC(ARCTH, atanh, arctanh, {Knot* div_knot = knot_op_creater (current_knot->prev, dR, nullptr, DIV);   \
                                R(div_knot) = knot_op_creater (div_knot, nullptr, nullptr, SUB);            \
                                RL(div_knot) = knot_num_creater (R(div_knot), nullptr, nullptr, 1);         \
                                RR(div_knot) = knot_op_creater (R(div_knot), RKNOT_CLONE, nullptr, POW);    \
                                RRR(div_knot) = knot_num_creater (RR(div_knot), nullptr, nullptr, 2);       \
                                KNOT = div_knot;})

DEF_FUNC(EXP, exp, e^,  {Knot* mul_knot = knot_op_creater (PKNOT, dR, KNOT_CLONE, MUL); \
                        KNOT = mul_knot;})

DEF_FUNC(SQRT, sqrt, sqrt,  {Knot* div_knot = knot_op_creater (PKNOT, dR, nullptr, DIV);                        \
                            R(div_knot) = knot_op_creater (div_knot, nullptr, KNOT_CLONE, MUL);                 \
                            div_knot->right->left = knot_num_creater (div_knot->right, nullptr, nullptr, 2);    \
                            KNOT = div_knot;})