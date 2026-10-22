/*
 * A346317  Number of permutations of [n] having two cycles of the form (c1,
 *          c2, ..., c_m) where c1 = min_{i>=1} c_i and c_j = min_{i>=j} c_i
 *          or c_j = max_{i>=j} c_i.
 * https://oeis.org/A346317
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n==2) * a(n) = 3 (n==3) * a(n) = 2^(2*n-5) +
 *            (n-1)*2^(n-4) (n>=4) * * The implementation uses exponentiation
 *            by squaring on seq_t values * to keep the number of
 *            multiplications small even for large n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

/* Compute 2^exp on type seq_t */
static inline seq_t pow2_idx(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp) {
        if (exp & (idx_t)1)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)2)
        return (seq_t)1;
    if (n == (idx_t)3)
        return (seq_t)3;

    /* For n >= 4 use the closed‑form expression */
    idx_t exp1 = (idx_t)(2 * n - 5); /* exponent for 2^(2*n-5) */
    idx_t exp2 = (idx_t)(n - 4);     /* exponent for 2^(n-4) */

    seq_t pow1 = pow2_idx(exp1);
    seq_t pow2 = pow2_idx(exp2);

    seq_t mult = seq_mul((seq_t)(n - 1), pow2);
    return pow1 + mult;
}
