/*
 * A314712  Coordination sequence Gal.6.150.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+2*a(n-5)-2*a(n-6) *
 *            +2*a(n-7)-2*a(n-8)+2*a(n-9)-2*a(n-10)+2*a(n-11)-a(n-12) * with
 *            a(0)=1, a(1)=5, a(2)=9, a(3)=13, a(4)=18, a(5)=22, a(6)=27, *
 *            a(7)=32, a(8)=36, a(9)=41, a(10)=45, a(11)=49, a(12)=54
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms   */
static size_t cache_cap = 0;         /* number of entries allocated      */
static idx_t cache_max = (idx_t)-1;  /* highest index currently stored    */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;      /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Initialize the first 13 terms in the cache. */
static void init_cache(void)
{
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)5;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)13;
    cache[4] = (seq_t)18;
    cache[5] = (seq_t)22;
    cache[6] = (seq_t)27;
    cache[7] = (seq_t)32;
    cache[8] = (seq_t)36;
    cache[9] = (seq_t)41;
    cache[10] = (seq_t)45;
    cache[11] = (seq_t)49;
    cache[12] = (seq_t)54;
    cache_max = 12;
}

seq_t a(idx_t n)
{
    /* Build cache up to n if necessary. */
    if (cache == NULL) {
        ensure_cache(n);
        init_cache();
    } else {
        ensure_cache(n);
        if (cache_max == (idx_t)-1) {
            init_cache();
        }
    }

    /* Return previously computed value if available. */
    if (n <= cache_max)
        return cache[n];

    /* Compute terms from cache_max+1 up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val -= 2 * cache[i - 2];
        val += 2 * cache[i - 3];
        val -= 2 * cache[i - 4];
        val += 2 * cache[i - 5];
        val -= 2 * cache[i - 6];
        val += 2 * cache[i - 7];
        val -= 2 * cache[i - 8];
        val += 2 * cache[i - 9];
        val -= 2 * cache[i - 10];
        val += 2 * cache[i - 11];
        val -= cache[i - 12];               /* last term coefficient is -1 */
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
