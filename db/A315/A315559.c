/*
 * A315559  Coordination sequence Gal.4.149.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A315559: Coordination sequence Gal.4.149.2
   Recurrence:  a(n) = 2*a(n-5) - a(n-10)   for n > 10
   Initial values:  a(0)=1, a(1)=6, a(2)=11, a(3)=18, a(4)=22,
                    a(5)=30, a(6)=34, a(7)=40, a(8)=47, a(9)=50, a(10)=60
   The function is memoised so that calls may occur in any order. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t   cache_size = 0;   /* number of allocated elements */
static idx_t   computed_max = (idx_t)-1; /* highest index computed */

/* initialise cache with the first 11 terms */
static void init_cache(void)
{
    if (cache != NULL) return;     /* already initialised */

    /* allocate space for indices 0..10 */
    cache_size = 11;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (!cache) abort();

    /* set the first eleven terms explicitly */
    cache[0] = 1;
    cache[1] = 6;
    cache[2] = 11;
    cache[3] = 18;
    cache[4] = 22;
    cache[5] = 30;
    cache[6] = 34;
    cache[7] = 40;
    cache[8] = 47;
    cache[9] = 50;
    cache[10] = 60;

    computed_max = 10;
}

/* Ensure cache can hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_size) return;

    idx_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size <= n) new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_size = new_size;
}

/* Compute a(n) for any n >= 0, memoising results. */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= computed_max) return cache[n];

    ensure_capacity(n);

    /* Compute missing terms sequentially. */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        if (i <= 10)
        {
            /* values 0..10 are already stored in cache after init_cache. */
            /* No action needed but keep the branch for clarity. */
            continue;
        }
        /* Recurrence: a(i) = 2*a(i-5) - a(i-10) */
        cache[i] = cache[i-5] * ((seq_t)2) - cache[i-10];
    }

    computed_max = n;
    return cache[n];
}
