/*
 * A242238  Number of length n+7+1 0..7 arrays with every value 0..7 appearing
 *          at least once in every consecutive 7+2 elements, and new values
 *          0..7 introduced in order.
 * https://oeis.org/A242238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 112,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242238
   Formula: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)
   (for n >= 9, with a(1)=36, a(2)=92, a(3)=197, a(4)=400, a(5)=799,
    a(6)=1590, a(7)=3165, a(8)=6308) */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;          /* array of computed values, 1‑indexed */
static idx_t  cache_max = 0;         /* highest index that has a value */

/* Ensure cache holds all values up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* Allocate larger array */
    size_t new_cap = (size_t)(n + 1);          /* index 0 unused */
    seq_t *new_cache = seq_alloc_new_array(new_cap);

    /* Copy existing data */
    for (idx_t i = 1; i <= cache_max; ++i)
        new_cache[i] = cache[i];

    /* If we had an old array, free it */
    if (cache != NULL)
        free_seq_array(cache);

    cache = new_cache;

    /* Initialise first 8 terms if they are not yet set */
    static const seq_t init[9] = {0, 36, 92, 197, 400, 799, 1590, 3165, 6308};
    for (idx_t i = cache_max + 1; i <= 8 && i <= n; ++i)
        cache[i] = init[i];

    /* Compute remaining terms using the recurrence */
    for (idx_t i = (cache_max > 8 ? cache_max + 1 : 9); i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 8; ++k)
            sum += cache[i - k];
        cache[i] = sum;
    }

    /* Update the current maximum */
    if (n > cache_max)
        cache_max = n;
}

seq_t a(idx_t n)
{
    /* We define a(1)=36.  For n<1 the sequence is undefined, but we return 0. */
    if (n < 1)
        return 0;
    ensure_cache(n);
    return cache[n];
}
