/*
 * A360774  Expansion of Sum_{k>=0} (x * (k + x))^k.
 * https://oeis.org/A360774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/2⌋} (n−k)^(n−2k) * binomial(n−k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   _pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_pow.h"

/* Helper: compute n choose k using multiplicative formula. */
static seq_t _binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Sequence function required by the project. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m   = n - k;
        idx_t exp = n - 2 * k;
        seq_t power = _pow((seq_t)m, exp);
        seq_t binom = _binom(m, k);
        sum += power * binom;
    }
    return sum;
}
