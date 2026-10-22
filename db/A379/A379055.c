/*
 * A379055  a(n) = composite(2*n+3) - composite(2*n+1).
 * https://oeis.org/A379055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite(2*n+3) - composite(2*n+1)
 * Range:     u8 n <= 307, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Cache for composite numbers (1‑based index). */
static uint64_t *comp_vals   = NULL;   /* cached composites */
static size_t   comp_cnt     = 0;       /* number of cached composites */
static size_t   comp_cap     = 0;       /* capacity of comp_vals */
static uint64_t next_candidate = 4;     /* next integer to test for compositeness */

/* Ensure comp_vals can hold at least `needed` elements. */
static void comp_ensure(size_t needed)
{
    if (needed <= comp_cap)
        return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(comp_vals, new_cap * sizeof(uint64_t));
    comp_vals = tmp;
    comp_cap  = new_cap;
}

/* Append a new composite number to the cache. */
static void comp_add(uint64_t v)
{
    if (comp_cnt == comp_cap)
        comp_ensure(comp_cnt + 1);
    comp_vals[comp_cnt++] = v;
}

/* Return the k‑th composite number (1‑based). */
static uint64_t comp_of(idx_t k)
{
    if (k == 0)               /* k==0 is not used in this sequence */
        return 0;
    /* Grow cache until it contains at least k composites. */
    while (comp_cnt < (size_t)k) {
        if (!is_prime_small(next_candidate))
            comp_add(next_candidate);      /* composite found */
        ++next_candidate;
    }
    return comp_vals[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    idx_t i1 = 2 * n + 1;
    idx_t i2 = 2 * n + 3;
    uint64_t c1 = comp_of(i1);
    uint64_t c2 = comp_of(i2);
    return (seq_t)(c2 - c1);
}
