/*
 * A288353  Strings of 3 digits from 1...9, such that no formula using the
 *          single digits in the given order exists that evaluates to 0.
 * https://oeis.org/A288353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 455, u32 n <= 455, u64 n <= 455,
 *            u128 n <= 455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search of all 3‑digit strings 111–999 that
   cannot be turned into an expression evaluating to 0 using the
   digits in the given order and the binary operators +, −, *, / and
   parentheses.  The list is generated once at first call and memoised. */

#include <stdbool.h>
#include <stdlib.h>

/* Rational type for intermediate results.  The numerator and denominator
   are stored as seq_t without reduction; we only test whether the
   numerator is zero.  Division by zero is avoided by explicit checks. */
typedef struct { seq_t num; seq_t den; } Rat;

/* Helper to create a rational from an integer. */
static inline Rat make_rat(seq_t v) { return (Rat){v, (seq_t)1}; }

static inline bool zero_expr(int d1, int d2, int d3)
{
    Rat a = make_rat((seq_t)d1);
    Rat b = make_rat((seq_t)d2);
    Rat c = make_rat((seq_t)d3);

    for (int op1 = 0; op1 < 4; ++op1) {
        for (int op2 = 0; op2 < 4; ++op2) {
            /* ((a op1 b) op2 c) */
            Rat t1, t2;  /* intermediate results */
            /* apply op1 between a and b */
            switch (op1) {
                case 0: t1.num = a.num * b.den + b.num * a.den; t1.den = a.den * b.den; break;
                case 1: t1.num = a.num * b.den - b.num * a.den; t1.den = a.den * b.den; break;
                case 2: t1.num = a.num * b.num;                t1.den = a.den * b.den; break;
                case 3: if (b.num == (seq_t)0) continue;
                        t1.num = a.num * b.den;        t1.den = a.den * b.num; break;
            }
            /* apply op2 between t1 and c */
            switch (op2) {
                case 0: t2.num = t1.num * c.den + c.num * t1.den; t2.den = t1.den * c.den; break;
                case 1: t2.num = t1.num * c.den - c.num * t1.den; t2.den = t1.den * c.den; break;
                case 2: t2.num = t1.num * c.num;                    t2.den = t1.den * c.den; break;
                case 3: if (c.num == (seq_t)0) continue;
                        t2.num = t1.num * c.den;        t2.den = t1.den * c.num; break;
            }
            if (t2.num == (seq_t)0) return true;

            /* a op1 (b op2 c) */
            Rat t3, t4;
            /* apply op2 between b and c */
            switch (op2) {
                case 0: t3.num = b.num * c.den + c.num * b.den; t3.den = b.den * c.den; break;
                case 1: t3.num = b.num * c.den - c.num * b.den; t3.den = b.den * c.den; break;
                case 2: t3.num = b.num * c.num;                t3.den = b.den * c.den; break;
                case 3: if (c.num == (seq_t)0) continue;
                        t3.num = b.num * c.den;        t3.den = b.den * c.num; break;
            }
            /* apply op1 between a and t3 */
            switch (op1) {
                case 0: t4.num = a.num * t3.den + t3.num * a.den; t4.den = a.den * t3.den; break;
                case 1: t4.num = a.num * t3.den - t3.num * a.den; t4.den = a.den * t3.den; break;
                case 2: t4.num = a.num * t3.num;                t4.den = a.den * t3.den; break;
                case 3: if (t3.num == (seq_t)0) continue;
                        t4.num = a.num * t3.den;        t4.den = a.den * t3.num; break;
            }
            if (t4.num == (seq_t)0) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The list of accepted numbers: those 3‑digit strings that admit
       no 0‑valued expression.  It is computed lazily once. */
    static seq_t list[729];
    static idx_t sz = 0;
    static bool ready = false;

    if (!ready) {
        idx_t idx = 0;
        for (int d1 = 1; d1 <= 9; ++d1)
            for (int d2 = 1; d2 <= 9; ++d2)
                for (int d3 = 1; d3 <= 9; ++d3) {
                    if (!zero_expr(d1, d2, d3)) {
                        list[idx++] = (seq_t)(d1 * 100 + d2 * 10 + d3);
                    }
                }
        sz = idx;
        ready = true;
    }

    if (n == (idx_t)0 || n > sz)
        return (seq_t)0;  /* undefined beyond the sequence length */

    return list[(idx_t)n - (idx_t)1];
}
