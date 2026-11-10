/*
 * A290289  The number of maximal subsemigroups of the monoid of partial
 *          orientation-preserving and reversing injective mappings on the set
 *          [1..n].
 * https://oeis.org/A290289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(n-1) + A008472(n) + 1 for n > 2; a(1) = a(2) = 2
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 883 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A290289_factor(uint64_t m, uint64_t *cnt, uint64_t *sum)
{
    *cnt = 0;
    *sum = 0;
    if (m % 2 == 0) {
        (*cnt)++;
        *sum += 2;
        while (m % 2 == 0)
            m /= 2;
    }
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            (*cnt)++;
            *sum += d;
            while (m % d == 0)
                m /= d;
        }
    }
    if (m > 1) {
        (*cnt)++;
        *sum += m;
    }
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)2;
    uint64_t c1, s1, c2, s2;
    A290289_factor((uint64_t)(n - 1), &c1, &s1);
    A290289_factor((uint64_t)n, &c2, &s2);
    return (seq_t)c1 + (seq_t)s2 + (seq_t)1;
}
