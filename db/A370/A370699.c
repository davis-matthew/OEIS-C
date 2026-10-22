/*
 * A370699  Expansion of e.g.f. exp(-x^3/6)/(1-x).
 * https://oeis.org/A370699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0,
   if n%3!=0: a(n)=n*a(n-1);
   else: a(n)=n*a(n-1)+(-1)^{n/3} * n!/(6^{n/3} (n/3)!) */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_prev = 1;   /* a(0) */
    seq_t fact   = 1;   /* factorial of i, starts at 0! */
    seq_t denom  = 1;   /* 6^0 * 0! */

    for (idx_t i = 1; i <= n; ++i) {
        fact *= i;      /* i! */

        seq_t a_val;
        if (i % 3 == 0) {
            idx_t k = i / 3;
            denom *= (6 * k);                /* denom = 6^k * k! */
            seq_t term = fact / denom;       /* n!/(6^k k!) */
            if (k % 2 == 0) {
                a_val = a_prev * i + term;
            } else {
                a_val = a_prev * i - term;
            }
        } else {
            a_val = a_prev * i;
        }
        a_prev = a_val;
    }
    return a_prev;
}
