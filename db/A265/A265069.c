/*
 * A265069  Coordination sequence for (2,6,6) tiling of hyperbolic plane.
 * https://oeis.org/A265069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 51, u64 n <= 105,
 *            u128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A265069: Coordination sequence for (2,6,6) tiling of hyperbolic plane
   Recurrence (for n >= 7):
     a(n) = a(n-1) + a(n-3) + a(n-5) - a(n-6)
   Initial values:
     a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=13, a(5)=21, a(6)=32
   The function caches computed values for efficiency. */
#include <stdlib.h>

static seq_t *cache   = NULL;   /* cache of computed terms */
static idx_t  cache_size = 0;   /* number of entries currently stored */

static void extend_cache(idx_t upto)
{
    if (cache_size > upto)
        return;  /* nothing to do */

    idx_t new_size = upto + 1;
    seq_t *mem = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!mem) abort();  /* allocation failure */
    cache = mem;

    /* Set the first seven terms (if within the new size) */
    if (new_size > 0) {
        seq_t base[7] = { (seq_t)1, (seq_t)3, (seq_t)5,
                          (seq_t)8, (seq_t)13, (seq_t)21,
                          (seq_t)32 };
        idx_t limit = new_size < 7 ? new_size : 7;
        for (idx_t i = 0; i < limit; ++i)
            cache[i] = base[i];
    }

    /* Compute terms from n = 7 up to 'upto' using the recurrence */
    for (idx_t i = 7; i <= upto; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        cache[i] = a1 + a3 + a5 - a6;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
