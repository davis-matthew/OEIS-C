/*
 * A246438  Numbers m such that A164349(m) = 0.
 * https://oeis.org/A246438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 0 (in increasing order) with A164349(m) = 0;
 *            computed by a linear scan over m, evaluating A164349 for each m
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8287 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A164349_a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    idx_t i = n;
    idx_t base = 1;
    idx_t k = 0;
    while (base <= i - 1) {
        base <<= 1;
        ++k;
    }

    while (k > 0) {
        idx_t lenPrev = (base >> 1) + 1;
        if (i >= lenPrev)
            i -= lenPrev;
        base >>= 1;
        --k;
    }

    return (seq_t)(i);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    for (;; ++m) {
        if (A164349_a(m) == 0) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
