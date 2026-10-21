/*
 * A268466  Smallest m > 1 such that m^m == 1 (mod n).
 * https://oeis.org/A268466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal m > 1 such that m^m ≡ 1 (mod n), with a(1) = 2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;   /* a(1) = 2 by definition */

    seq_t mod = (seq_t)n;
    /* According to the OEIS entry, a(n) ≤ n + (-1)^n, so m will not
       exceed n+1.  We simply search from 2 upwards. */
    for (idx_t m = 2; m <= n + 1; ++m) {
        if (mod_pow((seq_t)m, (uint64_t)m, mod) == (seq_t)1)
            return (seq_t)m;
    }
    /* In theory the loop should always find a solution within the
       bound above.  If it does not, fall back to a brute force search
       without the upper bound. */
    for (idx_t m = n + 2; ; ++m) {
        if (mod_pow((seq_t)m, (uint64_t)m, mod) == (seq_t)1)
            return (seq_t)m;
    }
}
