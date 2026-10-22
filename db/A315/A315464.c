/*
 * A315464  Coordination sequence Gal.6.346.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 Initial values:
 *            a(0) = 1 a(1) = 6 a(2) = 11 a(3) = 16 a(4) = 21 a(5) = 25 a(6) =
 *            31 a(7) = 35 a(8) = 40 a(9) = 45 a(10) = 50 a(11) = 56 a(12) =
 *            62 The function supports arbitrary ordering of calls and is
 *            memoized
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
/* C++: use reinterpret_cast for void* → seq_t* conversion */
#define ALLOC(n) reinterpret_cast<seq_t*>(malloc((n)))
#define REALLOC(p, n) reinterpret_cast<seq_t*>(realloc((p), (n)))
#else
/* C: simple cast is sufficient */
#define ALLOC(n) ((seq_t*)malloc((n)))
#define REALLOC(p, n) ((seq_t*)realloc((p), (n)))
#endif

/* Base terms for indices 0 … 12 */
static const seq_t base[13] = {
    (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)16, (seq_t)21,
    (seq_t)25, (seq_t)31, (seq_t)35, (seq_t)40, (seq_t)45,
    (seq_t)50, (seq_t)56, (seq_t)62
};

static seq_t *cache = NULL;      /* dynamic storage for computed terms */
static size_t cache_len = 0;     /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Return known base values directly */
    if (n < (idx_t)13)
        return base[(size_t)n];

    /* Ensure storage is large enough to hold indices 0 … n */
    if (cache == NULL || cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;

        if (cache == NULL) {
            cache = ALLOC(new_len * sizeof(seq_t));
            if (!cache) return (seq_t)0;  /* allocation failure unlikely */

            /* Initialise base values */
            for (size_t i = 0; i < 13; ++i)
                cache[i] = base[i];
        } else {
            seq_t *tmp = REALLOC(cache, new_len * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* allocation failure unlikely */
            cache = tmp;

            /* Initialise base values that were not previously present */
            if (cache_len < 13) {
                for (size_t i = cache_len; i < 13; ++i)
                    cache[i] = base[i];
            }
        }

        /* Compute any new terms using the recurrence */
        size_t start = (cache_len > 13) ? cache_len : 13;
        for (size_t i = start; i < new_len; ++i)
            cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
