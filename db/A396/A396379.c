/*
 * A396379  Fixed points of A396378: integers m so that m is the smallest
 *          integer of the interval (m, m+1, ..., m+A396377(m)) that divides
 *          the product of the other integers.
 * https://oeis.org/A396379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 with A396378(m) = m; found by search (extend
 *            interval m..r until some element divides the product of the
 *            others, test whether the smallest such is m)
 * Range:     u8 n <= 54, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A396379_vp(uint64_t x, uint64_t p)
{
    uint64_t e = 0;
    while (x % p == 0) { x /= p; ++e; }
    return e;
}

static bool A396379_isfixed(uint64_t m)
{
    for (uint64_t r = m + 1; ; ++r) {
        for (uint64_t c = m; c <= r; ++c) {
            uint64_t x = c;
            bool ok = true;
            for (uint64_t p = 2; p * p <= x || x > 1; ++p) {
                if (p * p > x) p = x;
                if (x % p) continue;
                uint64_t e = 0;
                while (x % p == 0) { x /= p; ++e; }
                uint64_t tot = 0;
                for (uint64_t j = m; j <= r; ++j) {
                    if (j % p == 0) tot += A396379_vp(j, p);
                }
                if (tot < 2 * e) { ok = false; break; }
            }
            if (ok) return c == m;
        }
    }
}

static uint64_t *A396379_terms = 0;
static size_t A396379_cnt = 0, A396379_cap = 0;
static uint64_t A396379_next = 1;

seq_t a(idx_t n)
{
    while (A396379_cnt < (size_t)n) {
        if (A396379_isfixed(A396379_next)) {
            if (A396379_cnt == A396379_cap) {
                A396379_cap = A396379_cap ? A396379_cap * 2 : 256;
                A396379_terms = (uint64_t *)realloc(A396379_terms, A396379_cap * sizeof(uint64_t));
            }
            A396379_terms[A396379_cnt++] = A396379_next;
        }
        ++A396379_next;
    }
    return (seq_t)A396379_terms[(size_t)n - 1];
}
