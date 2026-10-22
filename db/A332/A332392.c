/*
 * A332392  Number of minimal total dominating sets in the 2 X n king graph.
 * https://oeis.org/A332392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-2) + 5*a(n-3) + 4*a(n-4) + 8*a(n-5) + 16*a(n-6)
          - 8*a(n-7) - 48*a(n-8) - 64*a(n-9)  for n ≥ 9
   initial values:
   a(0)=1, a(1)=1, a(2)=6, a(3)=10, a(4)=15, a(5)=52,
   a(6)=105, a(7)=175, a(8)=481
*/

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* initialise cache for the first nine terms */
static void init_cache(void)
{
    if (cache == NULL) {
        /* Allocate space for at least the nine known terms */
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)52;
        cache[6] = (seq_t)105;
        cache[7] = (seq_t)175;
        cache[8] = (seq_t)481;
    }
}

/* grow the cache so that it can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_size) return;

    size_t new_size = cache_size ? cache_size : 16;
    while (new_size < needed) new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();                /* allocation failed */
    cache = tmp;
    cache_size = new_size;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* convert idx_t to size_t – safe for n up to 10 000 in the test suite */
    size_t i = (size_t)n;

    /* initialise cache on first call */
    init_cache();

    /* make sure cache is big enough */
    ensure_capacity(i + 1);

    /* compute missing terms */
    for (size_t k = 9; k <= i; ++k) {
        cache[k] =
            cache[k-2]
          + (seq_t)5 * cache[k-3]
          + (seq_t)4 * cache[k-4]
          + (seq_t)8 * cache[k-5]
          + (seq_t)16 * cache[k-6]
          - (seq_t)8  * cache[k-7]
          - (seq_t)48 * cache[k-8]
          - (seq_t)64 * cache[k-9];
    }

    return cache[i];
}
