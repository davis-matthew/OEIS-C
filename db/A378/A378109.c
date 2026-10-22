/*
 * A378109  Cogrowth sequence of the 18-element group S3 X C3 = <S,T,U | S^3,
 *          T^2, U^3, (ST)^2, [S,U], [T,U]>.
 * https://oeis.org/A378109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies a linear recurrence of order 8 with
 *    a(n)=3·a(n‑1)+3·a(n‑2)−8·a(n‑3)−12·a(n‑4)+24·a(n‑5)+9·a(n‑6)+9·a(n‑7)−27·a(n‑8)
 * for n ≥ 9.
 *
 * Initial values (n=0..8):
 * 1, 0, 1, 2, 3, 15, 32, 126, 351
 *
 * The code uses a cache that grows as needed, which works for any
 * idx_t/seq_t combination supplied by the build system.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Initial values of the sequence. */
static const seq_t init_vals[9] = {
    (seq_t)1,
    (seq_t)0,
    (seq_t)1,
    (seq_t)2,
    (seq_t)3,
    (seq_t)15,
    (seq_t)32,
    (seq_t)126,
    (seq_t)351
};

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_len)
        return;

    /* Allocate a fresh array of the required size. */
    seq_t *new_cache = alloc_seq_array(need);
    if (!new_cache)
    {
        abort(); /* allocation failed, terminate safely */
    }

    /* Copy any previously stored values into the new array. */
    for (size_t i = 0; i < cache_len; ++i)
    {
        new_cache[i] = cache[i];
    }

    /* Compute new terms using the recurrence. */
    for (size_t i = cache_len; i < need; ++i)
    {
        if (i < 9)
        {
            new_cache[i] = init_vals[i];
        }
        else
        {
            new_cache[i] =
                3 * new_cache[i - 1] + 3 * new_cache[i - 2] - 8 * new_cache[i - 3]
                - 12 * new_cache[i - 4] + 24 * new_cache[i - 5]
                + 9 * new_cache[i - 6] + 9 * new_cache[i - 7]
                - 27 * new_cache[i - 8];
        }
    }

    /* Free the old array (if any) and update the cache pointer. */
    if (cache != NULL)
    {
        free_seq_array(cache);
    }
    cache = new_cache;
    cache_len = need;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
