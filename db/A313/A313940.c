/*
 * A313940  Coordination sequence Gal.4.138.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-1) - a(n-3) + a(n-4) + a(n-5) - 2*a(n-6) * + a(n-7)
 *            + a(n-8) - a(n-9) + a(n-11) - a(n-12) * Initial values: *
 *            a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=23, a(5)=29, a(6)=34, *
 *            a(7)=38, a(8)=45, a(9)=52, a(10)=57, a(11)=60, a(12)=64
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[13] = {
    1, 5, 10, 16, 23, 29, 34, 38, 45, 52, 57, 60, 64
};

static seq_t *cache = NULL;   /* array holding computed terms          */
static size_t cache_size = 0; /* number of entries currently stored    */

/* Compute a(n) for n >= 0.  The function is thread-safe via lazy
 * initialization; it can be called in any order. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache) {
        cache = (seq_t *)malloc(13 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        memcpy(cache, init, 13 * sizeof(seq_t));
        cache_size = 13;
    }

    /* Expand cache and fill missing terms up to n */
    size_t idx = (size_t)n;
    if (cache_size <= idx) {
        cache = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */

        for (size_t i = cache_size; i <= idx; ++i) {
            /* Positive contributions */
            seq_t pos = cache[i - 1];
            pos += cache[i - 4];
            pos += cache[i - 5];
            pos += cache[i - 7];
            pos += cache[i - 8];
            pos += cache[i - 11];

            /* Negative contributions */
            seq_t neg = cache[i - 3];
            neg += cache[i - 6];
            neg += cache[i - 6];          /* -2*a(i-6)  */
            neg += cache[i - 9];
            neg += cache[i - 12];

            cache[i] = pos - neg;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
