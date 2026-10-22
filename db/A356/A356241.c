/*
 * A356241  a(n) is the number of distinct Fermat numbers dividing n.
 * https://oeis.org/A356241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 935, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of distinct Fermat numbers 2^(2^k)+1 that divide n */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;              /* n = 1,2 have no Fermat divisors */

    seq_t nt = (seq_t)n;                     /* promote n for arithmetic */
    seq_t cnt = 0;                           /* result */

    idx_t exp = 1;                           /* 2^0 = 1, will be doubled each loop */

    for (;;)
    {
        /* Fermat number F = 2^exp + 1 */
        seq_t f = seq_pow((seq_t)2, exp) + (seq_t)1;

        if (f > nt) break;                  /* no larger Fermat number can divide n */

        if ((nt % f) == 0) ++cnt;           /* count this divisor */

        if (exp > (idx_t)UINT64_MAX / 2) break;   /* avoid overflow on next doubling */
        exp <<= 1;                          /* next exponent: 2^(k+1) */
    }

    return cnt;
}
