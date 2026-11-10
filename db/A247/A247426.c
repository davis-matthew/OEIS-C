/*
 * A247426  Complement of A247425.
 * https://oeis.org/A247426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001950(n) if n is in A003233, else A001950(n) + 1;
 *            membership is tested directly: A003231(A001950(n)) ==
 *            A001950(A003231(n))
 * Range:     u8 n <= 3, u16 n <= 32, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t r247426_s(seq_t m) {
    return isqrt_seq(5 * m * m);
}

/* A001950(m) = floor((3m + floor(sqrt(5m^2)))/2) */
static seq_t r247426_a1950(seq_t m) {
    return (3 * m + r247426_s(m)) / 2;
}

/* A003231(m) = floor((5m + floor(sqrt(5m^2)))/2) */
static seq_t r247426_a3231(seq_t m) {
    return (5 * m + r247426_s(m)) / 2;
}

seq_t a(idx_t n) {
    seq_t k = (seq_t)n;
    seq_t base = r247426_a1950(k);
    seq_t lhs = r247426_a3231(base);
    seq_t rhs = r247426_a1950(r247426_a3231(k));
    if (lhs == rhs) return base;
    return base + 1;
}
