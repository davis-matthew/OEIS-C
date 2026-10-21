/*
 * A296778  Detour index of the n-folded cube graph.
 * https://oeis.org/A296778
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(2) = 1 a(n) = 2^(n-4) * ((2^n-2)^2 - (2^n-4)) for even n ≥ 4
 *            a(n) = 2^(n-4) * (2^n-2)^2 for odd n ≥ 3 (The odd‑n formula is
 *            equivalent to ((2^n-2)^2)/2 for n = 3.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 2U)
        return 1;

    /* pre‑compute 2^n */
    seq_t pow_n = pow2(n);

    if (n & 1U) {                /* odd n (n≥3) */
        seq_t t = pow_n - (seq_t)2;
        seq_t sq = t * t;
        if (n >= 4U) {
            return sq * pow2(n - 4U);
        } else {                /* n == 3 */
            return sq / (seq_t)2;   /* 2^(n-4) = 1/2 when n = 3 */
        }
    } else {                     /* even n (n≥4) */
        seq_t t  = pow_n - (seq_t)2;
        seq_t sq = t * t;
        seq_t sub = pow_n - (seq_t)4;
        seq_t diff = sq - sub;
        return diff * pow2(n - 4U);
    }
}
