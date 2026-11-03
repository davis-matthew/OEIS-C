/*
 * A319194  a(n) = Sum_{k=1..n} sigma_n(k).
 * https://oeis.org/A319194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * k^n (sum of sigma_n(k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 340 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t p = ipow((seq_t)k, (seq_t)n);          /* k^n */
        sum += p * (seq_t)(n / k);                     /* * floor(n/k) */
    }
    return sum;
}
