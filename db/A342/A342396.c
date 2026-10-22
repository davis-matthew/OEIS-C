/*
 * A342396  a(n) = Sum_{k=1..n} k^(n/gcd(k,n) - 1).
 * https://oeis.org/A342396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^(n/gcd(k,n) - 1) (Offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Euclidean GCD for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t g    = gcd_idx(k, n);
        idx_t exp  = n / g - 1;
        seq_t term = (exp == 0) ? 1 : big_pow((seq_t)k, (seq_t)exp);
        sum += term;
    }
    return sum;
}
