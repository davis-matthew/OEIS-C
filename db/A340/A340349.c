/*
 * A340349  a(n) is the smallest k such that A292849(k) = 2n-1.
 * https://oeis.org/A340349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) = least k with A292849(k) = 2n-1, i.e. least k such
 *            that the least j with A000120(j) = A000120(j*k) is j = 2n-1
 *            (only odd k, j need testing)
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int A340349_pop(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* returns 1 iff A292849(k) == m (m odd), checking odd j < m first */
static int A340349_check(uint64_t k, uint64_t m)
{
    uint64_t j;
    for (j = 1; j < m; j += 2) {
        if (A340349_pop(j) == A340349_pop(j * k))
            return 0;
    }
    return A340349_pop(m) == A340349_pop(m * k);
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n * 2 - 1;
    uint64_t k;
    for (k = 1;; k += 2) {
        if (A340349_check(k, m))
            return (seq_t)k;
    }
}
