/*
 * A355171  a(n) = Sum_{k=0..n} binomial(n, k + 1)*k!*(n + 1)!/(k + 2)!.
 * https://oeis.org/A355171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k + 1)*k!*(n + 1)!/(k + 2)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t binom_seq(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* (n + 1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fact *= (seq_t)i;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binom_seq(n, k + 1);         /* C(n, k + 1) */
        idx_t denom = (k + 1) * (k + 2);         /* (k + 1)(k + 2) */
        __uint128_t prod = ( __uint128_t )fact * ( __uint128_t )bin;
        prod /= ( __uint128_t )denom;
        sum += (seq_t)prod;
    }
    return sum;
}
