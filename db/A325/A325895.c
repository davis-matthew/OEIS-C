/*
 * A325895  The successive approximations up to 2^n for the 2-adic integer
 *          9^(1/5).
 * https://oeis.org/A325895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325895: successive 5th‑root approximations of 9 modulo powers of 2.
   Formula: a(0) = 0,
   for n > 0, let d = a(n-1)^5 - 9.
   If d is divisible by 2^n   -> a(n) = a(n-1),
   else                        -> a(n) = a(n-1) + 2^(n-1). */

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t cur = 0;   /* a(0) */
    if (n == 0)
        return cur;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow5 = big_pow(cur, 5);      /* cur^5 */
        seq_t diff = pow5 - (seq_t)9;      /* unsigned wrap handles negative numbers */

        /* Test whether diff is divisible by 2^i by checking lower i bits. */
        seq_t temp = diff;
        idx_t k;
        for (k = 0; k < i; ++k) {
            if ((temp & (seq_t)1) != 0)   /* odd -> not divisible by 2^i */
                break;
            temp >>= 1;
        }

        if (k < i) { /* not divisible */
            cur += ((seq_t)1 << (i - 1));
        }
        /* else cur stays unchanged */
    }
    return cur;
}
