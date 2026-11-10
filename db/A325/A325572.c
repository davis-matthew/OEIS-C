/*
 * A325572  Numbers n that have divisor d > 1 such that
 *          A048720(A065621(d),n/d) = n.
 * https://oeis.org/A325572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that some divisor d>1 of m has
 *            A048720(A065621(d), m/d) = m (search with trial-division
 *            divisors)
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 8775 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A325572_rev(uint64_t n)
{
    uint64_t low = n & (~n + 1);
    return n ^ ((n - low) << 1);
}

static uint64_t A325572_clmul(uint64_t x, uint64_t y)
{
    uint64_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static int A325572_test(uint64_t m)
{
    uint64_t i;
    for (i = 1; i * i <= m; i++) {
        if (m % i) continue;
        uint64_t j = m / i;
        if (i > 1 && A325572_clmul(A325572_rev(i), j) == m) return 1;
        if (j != i && j > 1 && A325572_clmul(A325572_rev(j), i) == m) return 1;
    }
    return 0;
}

static uint64_t *A325572_cache = 0;
static uint64_t A325572_cnt = 0, A325572_cap = 0, A325572_next = 2;

seq_t a(idx_t n)
{
    while (A325572_cnt < (uint64_t)n) {
        if (A325572_test(A325572_next)) {
            if (A325572_cnt == A325572_cap) {
                A325572_cap = A325572_cap ? A325572_cap * 2 : 1024;
                A325572_cache = (uint64_t *)realloc(A325572_cache, A325572_cap * sizeof(uint64_t));
            }
            A325572_cache[A325572_cnt++] = A325572_next;
        }
        A325572_next++;
    }
    return (seq_t)A325572_cache[n - 1];
}
