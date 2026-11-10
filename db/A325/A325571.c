/*
 * A325571  Composite numbers n that have no divisor d > 1 such that
 *          A048720(A065621(d),n/d) = n.
 * https://oeis.org/A325571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite m such that A048720(A065621(d), m/d) != m
 *            for every divisor d>1 of m; found by a search over m with a
 *            cached term list
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9333 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A325571_rev(uint64_t n)
{
    uint64_t low = n & (~n + 1);
    return n ^ ((n - low) << 1);
}

static uint64_t A325571_gf2(uint64_t i, uint64_t j)
{
    uint64_t r = 0;
    while (j) {
        if (j & 1) r ^= i;
        i <<= 1;
        j >>= 1;
    }
    return r;
}

static int A325571_hit(uint64_t d, uint64_t m)
{
    return A325571_gf2(A325571_rev(d), m / d) == m;
}

static uint64_t *A325571_terms = 0;
static uint64_t A325571_cnt = 0, A325571_cap = 0, A325571_next = 4;

seq_t a(idx_t n)
{
    while (A325571_cnt < (uint64_t)n) {
        uint64_t m = A325571_next++;
        int composite = 0, ok = 1;
        uint64_t d;
        for (d = 2; d * d <= m; d++) {
            if (m % d == 0) {
                composite = 1;
                if (A325571_hit(d, m) || A325571_hit(m / d, m)) { ok = 0; break; }
            }
        }
        if (!composite || !ok) continue;
        if (A325571_hit(m, m)) continue;
        if (A325571_cnt == A325571_cap) {
            A325571_cap = A325571_cap ? A325571_cap * 2 : 1024;
            A325571_terms = (uint64_t *)realloc(A325571_terms, A325571_cap * sizeof(uint64_t));
        }
        A325571_terms[A325571_cnt++] = m;
    }
    return (seq_t)A325571_terms[n - 1];
}
