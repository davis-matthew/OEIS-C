/*
 * A328792  Numbers that are not the difference between any triangular number
 *          and the largest square that does not exceed it.
 * https://oeis.org/A328792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328792 : Numbers that are NOT of the form
   t - floor(sqrt(t))^2 where t is a triangular number.
   a(n) is the n‑th nonnegative integer that never occurs as such a difference. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

#define EXCL_MAX 200000   /* must be >= the largest value needed for n <= 10000 */

/* A static buffer wide enough to hold the first 20000 terms of the sequence
   (safety for n up to about 10000). */
static seq_t  cache[20000];
static size_t cache_len = 0;

/* Exclusion bitset: bit i 1 → i occurs as a difference. */
static uint8_t excluded[EXCL_MAX];
static bool    excluded_init = false;

/* Keeps track of the next integer to examine when filling cache */
static idx_t   next_test = 0;

/* ------------------------------------------------------------------ */
/* Helper: fill the exclusion array once. */
static void init_excluded(void)
{
    if (excluded_init) return;
    for (idx_t k = 1;; ++k)
    {
        seq_t T   = (seq_t)k * (k + 1) / 2;   /* T_k = k(k+1)/2   */
        seq_t r   = floor_sqrt(T);             /* floor(sqrt(T))   */
        seq_t diff = T - r * r;                /* T_k - floor^2    */
        if (diff > (seq_t)EXCL_MAX) break;
        excluded[(size_t)diff] = 1;
    }
    excluded_init = true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not used by OEIS */

    /* Initialise exclusion set once */
    init_excluded();

    /* If the requested term is already cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Fill cache until we have at least n terms. */
    idx_t v;
    for (v = next_test; cache_len < n; ++v)
    {
        if ((size_t)v < EXCL_MAX && excluded[(size_t)v])
            continue;               /* v is excluded – skip it   */
        cache[cache_len++] = v;       /* store non‑excluded value */
    }

    /* Update where to continue the next time. */
    next_test = v;

    return cache[n - 1];
}
