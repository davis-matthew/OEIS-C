/*
 * A345100  a(n) = Sum_{k=1..n} k^floor(n/k).
 * https://oeis.org/A345100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^floor(n/k)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Validated: standard, pass, 58 terms through n = 4808 (2026-11-01)
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
        idx_t m = n / k;                 /* floor(n/k) */
        seq_t power = ipow((seq_t)k, (seq_t)m);
        sum += power;
    }
    return sum;
}
