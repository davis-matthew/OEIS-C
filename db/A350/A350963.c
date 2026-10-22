/*
 * A350963  Coordination sequence for the XXOXX tiling with respect to a tile
 *          of type R.
 * https://oeis.org/A350963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=9, a(2)=29, a(3)=42, a(4)=63, a(5)=75,
   a(6)=97, a(7)=106;
   a(n) = a(n-2) + a(n-4) - a(n-6)  for n ≥ 8
*/
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache[0..size-1] holds a(0)..a(size-1)  */
static size_t cache_size = 0;        /* number of elements stored in cache   */

static void init_cache(void)
{
    /* Initialise the first eight terms of the sequence */
    cache = (seq_t*)malloc(sizeof(seq_t) * 8);
    cache[0] = 1; cache[1] = 9; cache[2] = 29; cache[3] = 42;
    cache[4] = 63; cache[5] = 75; cache[6] = 97; cache[7] = 106;
    cache_size = 8;
}

seq_t a(idx_t n)
{
    /* Ensure we have the initial cache ready */
    if (cache_size == 0) init_cache();

    /* Return if already computed */
    if ((size_t)n < cache_size) return cache[n];

    /* Extend the cache to accommodate index n */
    size_t needed = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) return 0;   /* allocation failed – return 0 */
    cache = new_cache;

    /* Compute missing terms using the linear recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        /* For i ≥ 8 the recurrence holds */
        if (i >= 8)
            cache[i] = cache[i - 2] + cache[i - 4] - cache[i - 6];
        /* For i < 8 the values were already set */
    }
    cache_size = needed;

    return cache[n];
}
