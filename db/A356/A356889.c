/*
 * A356889  a(n) = (n^2 + 3*n + 10/3)*4^(n-3) - 1/3.
 * https://oeis.org/A356889
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356889
   Formula: a(n) = (4^n*(10 + 3*n*(3 + n)) - 64)/192
   (Offset 2 – a(2) is the first listed term)
*/

#include <stdlib.h>

/* local binary‑exponentiation routine for 4^exp in the type seq_t */
static seq_t pow4(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* sequence is undefined below offset */

    /* 10 + 3*n*(3 + n) = 10 + 3*n*n + 9*n */
    seq_t tn   = (seq_t)n;
    seq_t t    = (seq_t)10 + (seq_t)3 * tn * tn + (seq_t)9 * tn;

    seq_t four_pow = pow4(n);
    seq_t num = four_pow * t - (seq_t)64;

    return num / (seq_t)192;
}
