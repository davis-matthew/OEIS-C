/*
 * A318734  a(n) = Sum_{k=1..n} (-1)^(k + 1) * d(2*k - 1), where d(k) is the
 *          number of divisors of k (A000005).
 * https://oeis.org/A318734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * d(2k-1) (d = number of
 *            divisors)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9979 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t d = num_divisors(2 * k - 1);
        seq_t term = (k & 1ULL) ? (seq_t)d : -(seq_t)d;
        sum += term;
    }
    return sum;
}
