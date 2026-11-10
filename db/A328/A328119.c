/*
 * A328119  Numbers n for which A276086(n) > A002620(n).
 * https://oeis.org/A328119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m >= 0 with A276086(m) > A002620(m), where
 *            A002620(m) = floor(m^2/4); found by scanning m upward with a
 *            saturating primorial-base product
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9648 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static int A328119_test(uint64_t m)
{
    static const uint32_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    const uint64_t CAP = ((uint64_t)1) << 62;
    uint64_t t = (m >> 1) * ((m + 1) >> 1);   /* A002620(m) */
    uint64_t rem = m;
    uint64_t res = 1;
    int big = 0;
    for (int k = 0; k < 16 && rem > 0; ++k) {
        uint64_t p = pr[k];
        uint64_t d = rem % p;
        rem /= p;
        for (uint64_t j = 0; j < d; ++j) {
            if (res > CAP / p) { big = 1; break; }
            res *= p;
        }
        if (big) break;
    }
    if (big) return 1;
    return res > t;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_m = 0;

    while (cnt < (size_t)n) {
        if (A328119_test(next_m)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = next_m;
        }
        ++next_m;
    }
    return (seq_t)terms[(size_t)n - 1];
}
