/*
 * A296946  Number of n X 2 0..1 arrays with each 1 adjacent to 1, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for OEIS A296946:
   a(1)=2, a(2)=8, a(3)=16, a(4)=36
   a(n) = 2*a(n-1) + 8*a(n-3) - 12*a(n-4)   for n≥5
   (OEIS offset 1) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

static seq_t *memo = NULL;      /* cache for computed values */
static size_t cache_size = 0;   /* current allocated capacity */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* arrays start at index 1, index 0 is unused */
    if ((idx_t)cache_size > n)
        return;

    size_t new_size = cache_size ? cache_size : 1;
    while ((idx_t)new_size <= n)
        new_size <<= 1;

    seq_t *new_mem = seq_realloc(memo, cache_size, new_size);
    if (!new_mem)
        abort();          /* allocation failed */

    /* Zero the newly allocated part of the cache */
    for (size_t i = cache_size; i < new_size; ++i)
        new_mem[i] = 0;

    memo      = new_mem;
    cache_size = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 */
    if (n == 0)
        return 0;

    ensure_cache(n);

    for (idx_t i = 1; i <= n; ++i) {
        if (memo[i] != 0)        /* already computed */
            continue;

        if (i == 1)
            memo[i] = 2;
        else if (i == 2)
            memo[i] = 8;
        else if (i == 3)
            memo[i] = 16;
        else if (i == 4)
            memo[i] = 36;
        else {
            /* Recurrence: a(i) = 2*a(i-1) + 8*a(i-3) - 12*a(i-4) */
            seq_t term1 = memo[i - 1] * (seq_t)2;
            seq_t term2 = memo[i - 3] * (seq_t)8;
            seq_t term3 = memo[i - 4] * (seq_t)12;
            memo[i] = term1 + term2 - term3;   /* unsigned wrap‑around is fine */
        }
    }
    return memo[n];
}
