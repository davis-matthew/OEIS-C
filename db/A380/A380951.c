/*
 * A380951  a(n) = 2^(2*n - HammingWeight(n)) * [x^n] ((1 - x)^(-5/2) - (x -
 *          1)^(-2)).
 * https://oeis.org/A380951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n - HammingWeight(n)) * [x^n]((1-x)^(-5/2) -
 *            (x-1)^(-2))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>

/* popcount for idx_t (uint64_t) */
static inline int popcount_idx(idx_t v) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)v);
#else
    int c = 0;
    while (v) {
        v &= v - 1;
        c++;
    }
    return c;
#endif
}

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return (seq_t)0;

    /* Hamming weight of n */
    int h = popcount_idx(n);

    /* Power of two factor: 2^(2*n - h) */
    idx_t exp_pow = 2 * n - h;
    seq_t pow_val = pow2(exp_pow);

    /* t = 2^(n - h) * prod_{k=1..n} (2k+3) / k  */
    seq_t t = pow2(n - h);
    for (idx_t k = 1; k <= n; ++k) {
        t *= (seq_t)(2 * k + 3);
        t /= (seq_t)k;
    }

    /* Result: a(n) = t - pow_val * (n+1) */
    return t - pow_val * (seq_t)(n + 1);
}
