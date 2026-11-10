/*
 * A343647  Inverse permutation to A343646.
 * https://oeis.org/A343647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0 with F(k) <= n < F(k+1), k > 1 (F =
 *            Fibonacci): a(n) = F(k+1) - 1 - a(n - F(k)). This is the inverse
 *            of A343646
 * Range:     u8 n <= 232, u16 n <= 10945, u32 n <= 10945, u64 n <= 10945,
 *            u128 n <= 10945, big any n
 * Validated: standard, pass, 84 terms through n = 9803 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    unsigned __int128 v = (unsigned __int128)n;
    unsigned __int128 res = 0;
    bool plus = true;
    while (v > 0) {
        unsigned __int128 f = 1, g = 2;
        while (g <= v) {
            unsigned __int128 t = f + g;
            f = g;
            g = t;
        }
        unsigned __int128 term = g - 1;
        if (plus) res += term; else res -= term;
        plus = !plus;
        v -= f;
    }
    return (seq_t)res;
}
