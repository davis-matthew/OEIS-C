/*
 * A338249  Nonpositive values in A053985, in order of appearance and negated.
 * https://oeis.org/A338249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = -A053985(A053754(n+1)), where A053754(n+1) is the n-th
 *            integer with even binary length (0 first) and A053985 replaces
 *            2^k by (-2)^k in binary
 * Range:     u8 n <= 170, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 79 terms through n = 8391 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t r = n - 1;
    idx_t t = 1;
    idx_t k;
    while (1) {
        idx_t block = ((idx_t)1) << (2 * t - 1);
        if (r < block) { k = block + r; break; }
        r -= block;
        ++t;
    }
    seq_t pos = 0, neg = 0, term = 1;
    int i = 0;
    while (k) {
        if (k & 1) {
            if (i & 1) pos += term; else neg += term;
        }
        term = term * 2;
        k >>= 1;
        ++i;
    }
    return pos - neg;
}
