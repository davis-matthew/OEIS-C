/*
 * A391115  Numbers that are neither 4-free nor 4-full.
 * https://oeis.org/A391115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the sequence consists of integers that have at least one prime
 *            factor with exponent ≥ 4 and at least one prime factor with
 *            exponent ≤ 3 (i.e., a mixed exponent). a(1) = 48, a(2) = 80, …
 * Range:     u8 n <= 12, u16 n <= 4936, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9803 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool has_mixed_exponents(uint64_t x)
{
    if (x < 2) return false;
    bool ge4 = false, le3 = false;
    uint64_t t = x;

    /* factor 2 */
    if ((t & 1) == 0) {
        int e = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++e;
        }
        if (e >= 4) ge4 = true; else le3 = true;
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= t; d += 2) {
        if (t % d == 0) {
            int e = 0;
            while (t % d == 0) {
                t /= d;
                ++e;
            }
            if (e >= 4) ge4 = true; else le3 = true;
            if (ge4 && le3) return true;
        }
    }

    if (t > 1) le3 = true;
    return ge4 && le3;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static uint64_t last = 1;

    if (n == 0) return 0;  /* undefined for n = 0 */

    while (len < n) {
        ++last;
        if (has_mixed_exponents(last)) {
            if (len == cap) {
                cap = (cap == 0) ? 8 : cap * 2;
                seq = mem_resize(seq_t, seq, cap);
            }
            seq[len++] = (seq_t)last;
        }
    }
    return seq[n - 1];
}
