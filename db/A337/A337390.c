/*
 * A337390  Expansion of sqrt((1-2*x+sqrt(1-12*x+4*x^2)) / (2 *
 *          (1-12*x+4*x^2))).
 * https://oeis.org/A337390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337390.
   Formula: a(n) = Σ_{k=0..n} 2^(n−k) * C(2k,k) * C(2n,2k)
   where C(n,k) is the binomial coefficient.  The implementation
   uses the recurrences
        C(2k,k)     = C(2(k−1),k−1) * (4k−2)/k
        C(2n,2k)    = C(2n,2(k−1)) * (2n−2k+2)(2n−2k+1)/(2k)(2k−1)
   and iterates k from 0 to n.  All arithmetic is integer arithmetic on
   the type seq_t, which can be a builtin unsigned integer type or an
   arbitrary‑precision integer class.  No dynamic memory is used.
*/

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= 2;                     /* pow = 2^n */

    seq_t total = 0;                  /* running sum */
    seq_t C = 1;      /* C(2k,k)  */
    seq_t D = 1;      /* C(2n,2k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 2^(n−k) * C(2k,k) * C(2n,2k) */
        total += pow * C * D;

        if (k == n)                /* last term – no further updates */
            break;

        /* update C(2(k+1),k+1) */
        seq_t factor_num   = (seq_t)4 * (k + 1) - 2;    /* 4*(k+1)-2 */
        seq_t factor_den   = k + 1;                     /* (k+1)   */
        C = C * factor_num / factor_den;

        /* update D = C(2n,2(k+1)) */
        seq_t t1 = (seq_t)2 * n;                 /* 2n                 */
        seq_t t2 = (seq_t)2 * k;                 /* 2k                 */
        seq_t num1 = t1 - t2;                    /* 2n-2k              */
        seq_t num  = num1 * (num1 - 1);          /* (2n-2k)(2n-2k-1)    */
        seq_t den1 = (seq_t)2 * (k + 1);         /* 2(k+1)             */
        seq_t den  = den1 * (den1 - 1);          /* (2(k+1))(2(k+1)-1) */
        D = D * num / den;

        /* prepare next 2^(n-(k+1)) by shifting right */
        pow >>= 1;
    }
    return total;
}
