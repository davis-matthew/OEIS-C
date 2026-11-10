/*
 * A373346  Row sums of A373345.
 * https://oeis.org/A373346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of the 1-based positions of the 1-bits of A371176(n),
 *            * where A371176 lists in increasing order the k >= 1 with *
 *            popcount(k) <= (1-based position of the lowest 1-bit of k). *
 *            Found by searching k upward; results are cached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9816 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A373346_sums = NULL;   /* row sums, 0-based by row */
static size_t    A373346_cnt  = 0;
static size_t    A373346_cap  = 0;
static uint64_t  A373346_next = 1;      /* next k to test */

static bool A373346_is_schreier(uint64_t k)
{
    if (k == 0) return false;
    unsigned pop = (unsigned)__builtin_popcountll(k);
    unsigned minpos = (unsigned)__builtin_ctzll(k) + 1;
    return pop <= minpos;
}

static uint32_t A373346_possum(uint64_t k)
{
    uint32_t s = 0;
    unsigned pos = 1;
    while (k) {
        if (k & 1) s += pos;
        k >>= 1;
        ++pos;
    }
    return s;
}

static void A373346_extend(size_t need)
{
    while (A373346_cnt < need) {
        uint64_t k = A373346_next;
        while (!A373346_is_schreier(k)) ++k;
        A373346_next = k + 1;
        if (A373346_cnt >= A373346_cap) {
            size_t nc = A373346_cap ? A373346_cap * 2 : 1024;
            uint32_t *p = (uint32_t *)realloc(A373346_sums, nc * sizeof(uint32_t));
            if (!p) abort();
            A373346_sums = p;
            A373346_cap = nc;
        }
        A373346_sums[A373346_cnt++] = A373346_possum(k);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A373346_extend((size_t)n);
    return (seq_t)A373346_sums[(size_t)n - 1];
}
