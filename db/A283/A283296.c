/*
 * A283296  Integers n such that floor(n^(3/2)) is cubefree.
 * https://oeis.org/A283296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that floor(k^(3/2)) is cubefree
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 720, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root, floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_pow.h"

static seq_t *seq_cache = NULL;       /* cached a(i) values */
static size_t seq_len   = 0;  /* number of cached values */
static size_t seq_cap   = 0;  /* allocated capacity of seq_cache */
static idx_t  next_k    = 1;  /* next integer k to test */

/* Test whether x is cubefree: no prime factor occurs with exponent ≥3. */
static bool is_cubefree(seq_t x)
{
    seq_t croot = floor_cube_root(x);
    if (croot <= (seq_t)1)
        return true;   /* 0 or 1 is trivially cubefree */

    idx_t bound = (idx_t)croot;   /* safe: croot is small for our range */
    for (idx_t p = 2; p <= bound; ++p)
    {
        seq_t p3 = seq_pow((seq_t)p, 3);   /* p^3 */
        if (x % p3 == 0)
            return false;
    }
    return true;
}

/* Ensure seq_cache has at least new_cap elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    if (seq_cap == 0)
        seq_cap = 16;
    while (seq_cap < new_cap)
        seq_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe */

    /* Compute terms until we have at least n of them. */
    while (seq_len < (size_t)n)
    {
        /* Compute floor(k^(3/2)) = floor(sqrt(k^3)). */
        seq_t k3  = seq_pow((seq_t)next_k, 3);         /* k^3 */
        seq_t b   = floor_sqrt(k3);                    /* floor(sqrt(k^3)) */

        if (is_cubefree(b))
        {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq_cache[n - 1];
}
