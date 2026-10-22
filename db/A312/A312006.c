/*
 * A312006  Coordination sequence Gal.4.23.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + a(n-7) - a(n-11), with a(0)=1, a(1)=4, a(2)=8,
 *            a(3)=13, a(4)=18, a(5)=20, a(6)=26, a(7)=30, a(8)=35, a(9)=37,
 *            a(10)=43, a(11)=48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i) for i < cache_cap */
static idx_t cache_size = 0;  /* number of initialized entries in cache */
static idx_t cache_cap   = 0;  /* allocated capacity of cache */

/* Allocate or enlarge the cache to hold at least 'need' entries. */
static void grow_cache(idx_t need)
{
    if (need < cache_cap)
        return;                  /* already sufficient */

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        abort();                 /* out of memory */

    /* Zero the newly allocated portion of the array. */
    if (new_cap > cache_cap)
        memset(&new_buf[cache_cap], 0,
               (new_cap - cache_cap) * sizeof(seq_t));

    cache = new_buf;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 12 known terms of the sequence. */
static void init_cache(void)
{
    if (cache)
        return;                  /* already initialised */

    cache_cap = 16;             /* sufficient for initial values */
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache)
        abort();                 /* out of memory */

    /* Set the first 12 terms explicitly. */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)8;
    cache[3] = (seq_t)13;
    cache[4] = (seq_t)18;
    cache[5] = (seq_t)20;
    cache[6] = (seq_t)26;
    cache[7] = (seq_t)30;
    cache[8] = (seq_t)35;
    cache[9] = (seq_t)37;
    cache[10] = (seq_t)43;
    cache[11] = (seq_t)48;

    cache_size = 12;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_size)
        return cache[n];        /* already known */

    /* Ensure the cache can hold the desired index. */
    if (n >= cache_cap)
        grow_cache(n);

    /* Compute missing entries up to and including n. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* Recurrence valid for i >= 12. */
        cache[i] = cache[i-4] + cache[i-7] - cache[i-11];
    }

    cache_size = n + 1;
    return cache[n];
}
