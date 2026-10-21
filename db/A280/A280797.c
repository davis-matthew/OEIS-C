/*
 * A280797  a(n) = (n^n - 1)*(n^n + 1)/(n + 1).
 * https://oeis.org/A280797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^n - 1)(n^n + 1)/(n + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* All computations are performed in seq_t to preserve precision. */
    seq_t sn = (seq_t)n;                      /* convert index to seq_t */
    seq_t x  = big_pow(sn, sn);               /* x = n^n */

    /* Since n ≡ -1 (mod n+1), one of (x-1) or (x+1) is divisible by n+1.
       Divide first to avoid overflow: */
    if ((sn & (seq_t)1) == (seq_t)0) {        /* n is even  → divide (x-1) */
        seq_t div = (x - (seq_t)1) / (sn + (seq_t)1);
        return div * (x + (seq_t)1);
    } else {                                 /* n is odd  → divide (x+1) */
        seq_t div = (x + (seq_t)1) / (sn + (seq_t)1);
        return div * (x - (seq_t)1);
    }
}
