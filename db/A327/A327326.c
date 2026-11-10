/*
 * A327326  a(n) = A006218(n) - A005187(n).
 * https://oeis.org/A327326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006218(n) - A005187(n), where A006218(n) = Sum_{k=1..n}
 *            floor(n/k) = 2*Sum_{i=1..floor(sqrt(n))} floor(n/i) -
 *            floor(sqrt(n))^2 and A005187(n) = Sum_{k>=0} floor(n/2^k)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A006218_isqrt(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x = n;
    uint64_t y = (x >> 1) + (x & 1);
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

static seq_t A006218_a(idx_t n) {
    if (n == 0) return 0;
    uint64_t s = A006218_isqrt((uint64_t)n);
    seq_t result = 0;
    for (uint64_t i = 1; i <= s; i++) {
        result += (seq_t)n / (seq_t)i;
    }
    result = result * 2 - (seq_t)s * (seq_t)s;
    return result;
}

static seq_t A005187_a(idx_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    return A006218_a(n) - A005187_a(n);
}
