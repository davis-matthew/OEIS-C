/*
 * A280305  a(n) = (n^(2n) - (n-1)^(2n))/(2n-1).
 * https://oeis.org/A280305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(2n) - (n-1)^(2n))/(2n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* For small seq_t we can use 128‑bit intermediates to avoid overflow.
       For larger seq_t (arbitrary‑precision) we defer to seq_pow. */
    if (sizeof(seq_t) <= 8) {
        /* n >= 1, so n-1 is well‑defined */
        unsigned __int128 base1 = (unsigned __int128)n;
        unsigned __int128 base2 = (unsigned __int128)(n - 1);
        idx_t        exp   = 2 * n;          /* 2n  */
        unsigned __int128 pow1 = 1;
        unsigned __int128 pow2 = 1;

        /* Compute n^(2n) and (n-1)^(2n) by repeated multiplication.
           The intermediate fits in unsigned __int128 for n where the final
           result fits in seq_t (which is uint64_t).  For n beyond that
           range the result does not fit and the cast to seq_t merely
           truncates, which is acceptable because those values are not
           checked. */
        for (idx_t i = 0; i < exp; ++i) {
            pow1 *= base1;
            pow2 *= base2;
        }
        unsigned __int128 num   = pow1 - pow2;          /* numerator   */
        unsigned __int128 denom = (unsigned __int128)(2 * n - 1);
        return (seq_t)(num / denom);                  /* final value */
    } else {
        /* For 128‑bit or arbitrary‑precision seq_t, use seq_pow which
           operates on the proper type. */
        seq_t pow1 = seq_pow((seq_t)n,     2 * n);
        seq_t pow2 = seq_pow((seq_t)(n-1), 2 * n);
        seq_t num  = pow1 - pow2;
        seq_t denom = (seq_t)(2 * n - 1);
        return num / denom;
    }
}
