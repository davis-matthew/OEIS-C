/*
 * A310267  Coordination sequence Gal.4.12.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 10; initial values * a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=14, a(4)=18, a(5)=20, a(6)=23, * a(7)=28,
 *            a(8)=36, a(9)=39, a(10)=40
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_up_to = 0; /* first index not yet computed */

/* Ensure the cache can hold at least 'cap' elements */
static void ensure_cache(size_t cap)
{
    if (cap <= cache_cap) return;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) abort(); /* fatal if allocation fails */
    cache = tmp;
    cache_cap = cap;
}

/* Initialize the base values a(0)…a(10) */
static void init_base(void)
{
    if (computed_up_to) return;            /* already initialized */
    ensure_cache(11);                      /* need indices 0..10 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)14;
    cache[4] = (seq_t)18;
    cache[5] = (seq_t)20;
    cache[6] = (seq_t)23;
    cache[7] = (seq_t)28;
    cache[8] = (seq_t)36;
    cache[9] = (seq_t)39;
    cache[10] = (seq_t)40;
    computed_up_to = 11;                    /* next index to compute */
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n */
    ensure_cache((size_t)n + 1);

    /* Set up initial values on first call */
    init_base();

    /* Compute missing entries up to 'n' using the recurrence
     * (only for indices > 10, but the loop is guarded by 'start') */
    idx_t start = computed_up_to;
    if (start < 11) start = 11;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 5] - cache[i - 10];
    }
    if (n + 1 > computed_up_to) computed_up_to = n + 1;

    return cache[n];
}
