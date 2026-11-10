/*
 * A276002  Numbers n for which A060502(n) = 2; numbers with exactly two
 *          occupied slopes in their factorial representation.
 * https://oeis.org/A276002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 with A060502(k) = 2, i.e. exactly two
 *            distinct values of i - d_i over nonzero factorial-base digits
 *            d_i (search with memoized scan)
 * Range:     u8 n <= 99, u16 n <= 4430, u32 n <= 14609, u64 n <= 14609,
 *            u128 n <= 14609, big any n
 * Validated: standard, pass, 98 terms through n = 9639 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *A276002_cache = 0;
static idx_t A276002_cnt = 0;
static idx_t A276002_cap = 0;
static idx_t A276002_next = 1;

static int A276002_slopes(idx_t m)
{
    uint32_t mask = 0;
    unsigned i = 1;
    while (m > 0) {
        unsigned d = (unsigned)(m % (i + 1));
        m /= (i + 1);
        if (d != 0)
            mask |= (uint32_t)1 << (i - d);
        ++i;
    }
    int c = 0;
    while (mask) {
        mask &= mask - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    while (A276002_cnt < n) {
        if (A276002_cnt == A276002_cap) {
            idx_t nc = A276002_cap ? A276002_cap * 2 : 1024;
            A276002_cache = (idx_t *)realloc(A276002_cache, nc * sizeof(idx_t));
            A276002_cap = nc;
        }
        while (A276002_slopes(A276002_next) != 2)
            ++A276002_next;
        A276002_cache[A276002_cnt++] = A276002_next++;
    }
    return (seq_t)A276002_cache[n - 1];
}
