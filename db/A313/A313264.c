/*
 * A313264  Coordination sequence Gal.5.216.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-4) + a(n-5) + a(n-7) - 2*a(n-8) + a(n-9) +
 *            a(n-11) - a(n-12) + a(n-15) - a(n-16) (n > 19) Initial values:
 *            a(0)=1, a(1)=4, a(2)=9, a(3)=15, a(4)=20, a(5)=25, a(6)=32,
 *            a(7)=36, a(8)=40, a(9)=47, a(10)=52, a(11)=56, a(12)=62,
 *            a(13)=68, a(14)=72, a(15)=76, a(16)=82, a(17)=87, a(18...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[20] = {
    (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)15, (seq_t)20,
    (seq_t)25, (seq_t)32, (seq_t)36, (seq_t)40, (seq_t)47,
    (seq_t)52, (seq_t)56, (seq_t)62, (seq_t)68, (seq_t)72,
    (seq_t)76, (seq_t)82, (seq_t)87, (seq_t)92, (seq_t)98
};

static seq_t *cache = NULL;      /* cached sequence values          */
static size_t cache_cap = 0;     /* allocated capacity (in elements) */
static size_t cache_size = 0;    /* number of elements currently cached */

/* Ensure the cache can hold indices up to 'needed-1'. */
static void grow_cache_to(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t old_cap = cache_cap;
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < needed)
        cache_cap <<= 1;

    if (cache == NULL)
        cache = seq_alloc(cache_cap);
    else
        cache = seq_realloc(cache, old_cap, cache_cap);

    if (!cache)
        abort();
}

/* Compute a(n) for the OEIS sequence A313264. */
seq_t a(idx_t n)
{
    /* Make sure the cache contains up to n. */
    if ((size_t)n + 1 > cache_size)
    {
        grow_cache_to((size_t)n + 1);

        for (size_t i = cache_size; i <= (size_t)n; ++i)
        {
            if (i < 20)
            {
                cache[i] = init_vals[i];
            }
            else
            {
                /* Apply the recurrence. All operations are on seq_t. */
                seq_t val = cache[i - 1];
                val -= cache[i - 4];
                val += cache[i - 5];
                val += cache[i - 7];
                val -= (seq_t)2 * cache[i - 8];
                val += cache[i - 9];
                val += cache[i - 11];
                val -= cache[i - 12];
                val += cache[i - 15];
                val -= cache[i - 16];
                cache[i] = val;
            }
        }
        cache_size = (size_t)n + 1;
    }
    return cache[n];
}
