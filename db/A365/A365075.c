/*
 * A365075  Decimal expansion of the initial irrational number of Cantor's
 *          diagonal argument: the k-th decimal digit of this constant is
 *          equal to the k-th decimal digit of A182972(k)/A182973(k).
 * https://oeis.org/A365075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(10*(p*10^n mod q)/q) where p/q =
 *            A182972(n+1)/A182973(n+1); computed by direct enumeration of
 *            reduced fractions and long division
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A365075_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

seq_t a(idx_t n) {
    uint64_t k = (uint64_t)n + 1;
    uint64_t count = 0, p = 0, q = 0;
    for (uint64_t sum = 3; count < k; ++sum) {
        for (uint64_t x = 1; x * 2 < sum; ++x) {
            if (A365075_gcd(x, sum - x) == 1) {
                if (++count == k) { p = x; q = sum - x; break; }
            }
        }
    }
    uint64_t r = p % q;
    for (uint64_t i = 1; i < k; ++i) r = (r * 10) % q;
    return (seq_t)((r * 10) / q);
}
