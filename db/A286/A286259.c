/*
 * A286259  Compound filter: a(n) = P(A001511(n), A049820(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + (A001511(n)+A049820(n))^2 - A001511(n) -
 *            3*A049820(n)), with A001511(n) = 2-adic valuation of 2n and
 *            A049820(n) = n - d(n)
 * Range:     u8 n <= 27, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9012 (2026-11-08)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

static seq_t A001511_a(idx_t n) {
    seq_t result = 1;
    while ((n & 1) == 0) {
        result++;
        n >>= 1;
    }
    return result;
}

static seq_t A049820_a(idx_t n) {
    if (n < 1) return 0;
    seq_t nd = (seq_t)n;
    seq_t d  = (seq_t)num_divisors((uint64_t)n);
    return nd - d;
}

seq_t a(idx_t n) {
    seq_t x = A001511_a(n);
    seq_t y = A049820_a(n);
    seq_t s = x + y;
    return (s * s + 2 - x - 3 * y) / 2;
}
