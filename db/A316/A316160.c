/*
 * A316160  Number of pairs of compositions of n corresponding to a seaweed
 *          algebra of index n-3.
 * https://oeis.org/A316160
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n^2 + 11*n - 25) * 2^(n-5) for n ≥ 5, with a(3) = 6
 *            and a(4) = 26
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)26;

    /* For n ≥ 5 use the closed‑form expression. */
    /* Safely compute the coefficient in 64‑bit arithmetic first. */
    unsigned long long k = (unsigned long long)n;
    seq_t coef = (seq_t)(2ULL * k * k + 11ULL * k - 25ULL);

    seq_t res = pow2(n - 5);   /* 2^(n-5) */
    res *= coef;               /* multiply the coefficient */
    return res;
}
