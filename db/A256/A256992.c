/*
 * A256992  Position of n in either of the complementary sequences, A005187 or
 *          A055938: a(n) = A213714(n) + A234017(n).
 * https://oeis.org/A256992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k if n = A005187(k) = 2k - popcount(k) for some k >= 1,
 *            otherwise a(n) = n - (number of k >= 1 with A005187(k) < n)
 * Range:     u8 n <= 510, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9992 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A256992_f(idx_t k) {
    idx_t p = 0, t = k;
    while (t) { p += t & 1; t >>= 1; }
    return 2 * k - p;
}

seq_t a(idx_t n) {
    idx_t k = n / 2;
    if (k < 1) k = 1;
    while (A256992_f(k) < n) k++;
    if (A256992_f(k) == n) return (seq_t)k;
    return (seq_t)(n - (k - 1));
}
