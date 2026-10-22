/*
 * A342379  Expansion of e.g.f. (exp(x)-1)*(exp(x) - x^3/6 - x^2/2 - x - 1).
 * https://oeis.org/A342379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - (1 + n + C(n,2) + C(n,3)) - ([n>=4] ? 1 : 0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This equals the number of binary strings of length n containing at least four 0's but not all zeros. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* power of two part */
    seq_t pow = pow2(n);

    /* binomial terms, avoiding underflow for n<2 or n<3 */
    seq_t c1 = (seq_t)n;
    seq_t c2 = 0;
    seq_t c3 = 0;

    if (n >= 2) {
        c2 = ((seq_t)n * ((seq_t)n - 1)) / 2;
    }
    if (n >= 3) {
        c3 = ((seq_t)n * ((seq_t)n - 1) * ((seq_t)n - 2)) / 6;
    }

    /* sum of first four binomials C(n,0)..C(n,3) */
    seq_t sum = 1 + c1 + c2 + c3;

    /* result */
    seq_t res = pow - sum;
    if (n >= 4) {
        res -= 1;
    }
    return res;
}
