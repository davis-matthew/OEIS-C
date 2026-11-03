/*
 * A349880  Expansion of Sum_{k>=0} x^k/(1 - k^3 * x).
 * https://oeis.org/A349880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(3*(n-k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 233 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t exp = 3 * (n - k);
        sum += ipow((seq_t)k, (seq_t)exp);
    }
    return sum;
}
