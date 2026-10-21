/*
 * A281741  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 393", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A281741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 256*a(n-8) - 256*a(n-10) for n > 23; Initial
 *            terms (n = 0..23) are
 *            1,0,5,0,21,0,85,8,321,28,1365,128,5141,448,21845,2176,81941,8128,348245,34560,1316181,115328,5591317,558144
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes values and expands the cache only as needed. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

static seq_t *cache       = NULL;   /* dynamic array of computed a(n) */
static size_t cache_cap   = 0;      /* allocated capacity (elements)   */
static size_t cache_max   = 0;      /* highest index that has a value  */

/* Constant initializer for the first 24 entries */
static const seq_t init_vals[24] = {
    (seq_t)1,   (seq_t)0,   (seq_t)5,   (seq_t)0,
    (seq_t)21,  (seq_t)0,   (seq_t)85,  (seq_t)8,
    (seq_t)321, (seq_t)28,  (seq_t)1365,(seq_t)128,
    (seq_t)5141,(seq_t)448, (seq_t)21845,(seq_t)2176,
    (seq_t)81941,(seq_t)8128,(seq_t)348245,(seq_t)34560,
    (seq_t)1316181,(seq_t)115328,(seq_t)5591317,(seq_t)558144
};

/* Ensure that the dynamic array has at least n+1 slots and that the initial
   24 values are filled in those slots. */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap) return;

    seq_t *tmp = seq_realloc(cache, cache_cap, need);
    cache    = tmp;
    cache_cap= need;

    /* guarantee that the first up-to-23 values are correct */
    size_t up_to = need-1;
    if (up_to > 23) up_to = 23;
    for (size_t i = 0; i <= up_to; ++i) cache[i] = init_vals[i];

    cache_max = up_to;   /* because we just (re)initialized the prefix */
}

/* Compute values from cache_max+1 up to n using the recurrence. */
static void compute_upto(idx_t n)
{
    ensure_capacity(n);
    if ((size_t)n <= cache_max) return;   /* already have everything */

    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        seq_t val = cache[i-2] + (seq_t)256 * cache[i-8] - (seq_t)256 * cache[i-10];
        cache[i] = val;
    }
    cache_max = (size_t)n;
}

/* Public function as required */
seq_t a(idx_t n)
{
    if ((size_t)n <= 23) {
        ensure_capacity(n);
        return cache[(size_t)n];
    }
    compute_upto(n);
    return cache[(size_t)n];
}
