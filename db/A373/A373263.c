/*
 * A373263  a(n) = 1 if A276085(n) == -1 (mod 3), otherwise 0, where A276085
 *          is the primorial base log-function.
 * https://oeis.org/A373263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (A007949(n) - A007814(n)) == 1 (mod 3), else 0; i.e.
 *            (v2(n) - v3(n)) mod 3 == 2
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9389 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    int e2 = 0, e3 = 0;
    while ((m & 1ULL) == 0) { e2++; m >>= 1; }
    while (m % 3ULL == 0) { e3++; m /= 3ULL; }
    int r = (((e2 - e3) % 3) + 3) % 3;
    return (seq_t)(r == 2 ? 1 : 0);
}
