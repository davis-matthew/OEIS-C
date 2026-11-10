/*
 * A255053  Least inverse of A255072; a(n) = smallest k such that A255072(k) =
 *          n.
 * https://oeis.org/A255053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with A255072(k) = n; computed by iterating k
 *            upward with memoized step counts (steps(k) = steps(k - runs(k))
 *            + 1)
 * Range:     u8 n <= 66, u16 n <= 8591, u32 n <= 16143, u64 n <= 16143,
 *            u128 n <= 16143, big any n
 * Validated: standard, pass, 83 terms through n = 8760 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A255053_st = NULL;
static uint64_t A255053_cnt = 0, A255053_cap = 0;
static uint64_t *A255053_first = NULL;
static uint64_t A255053_nf = 0, A255053_fcap = 0;

static unsigned A255053_runs(uint64_t k)
{
    uint64_t x = k ^ (k >> 1);
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A255053_cap == 0) {
        A255053_cap = 1024;
        A255053_st = (uint32_t *)malloc(A255053_cap * sizeof(uint32_t));
        A255053_fcap = 256;
        A255053_first = (uint64_t *)malloc(A255053_fcap * sizeof(uint64_t));
        A255053_st[0] = 0;
        A255053_cnt = 1;
        A255053_first[0] = 0;
        A255053_nf = 1;
    }
    while (A255053_nf <= (uint64_t)n) {
        uint64_t k = A255053_cnt;
        if (k >= A255053_cap) {
            A255053_cap *= 2;
            A255053_st = (uint32_t *)realloc(A255053_st, A255053_cap * sizeof(uint32_t));
        }
        uint32_t s = A255053_st[k - A255053_runs(k)] + 1;
        A255053_st[k] = s;
        A255053_cnt = k + 1;
        while (A255053_nf <= (uint64_t)s) {
            if (A255053_nf >= A255053_fcap) {
                A255053_fcap *= 2;
                A255053_first = (uint64_t *)realloc(A255053_first, A255053_fcap * sizeof(uint64_t));
            }
            A255053_first[A255053_nf++] = k;
        }
    }
    return (seq_t)A255053_first[n];
}
