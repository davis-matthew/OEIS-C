/*
 * A264545  Number of (n+1)X(3+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change -1,1 -1,2 1,0 or 0,-1.
 * https://oeis.org/A264545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +10*a(n-3) -4*a(n-4) +8*a(n-5) -25*a(n-6)
 *            -4*a(n-8) +33*a(n-9) +4*a(n-10) -4*a(n-11) -10*a(n-12)
 *            -2*a(n-13) +a(n-15) Initial values (1‑based): a(1)=1, a(2)=6,
 *            a(3)=16, a(4)=45, a(5)=133, a(6)=400, a(7)=1204, a(8)=3561,
 *            a(9)=10554, a(10)=31493, a(11)=93784, a(12)=278862,
 *            a(13)=8298...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached values, 1‑indexed (cache[0] is unused) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size (including index 0) */
    static idx_t  cached_up_to = 0; /* highest index already computed */

    /* initial values for n = 1..15 */
    static const seq_t init[16] = {
        0,              /* unused 0th index */
        1, 6, 16, 45, 133,
        400, 1204, 3561, 10554,
        31493, 93784, 278862,
        829809, 2469737, 7349616
    };

    /* extend cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;          /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(*cache));
        if (!tmp) return 0;                       /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* return if already known */
    if (n <= cached_up_to)
        return cache[(size_t)n];

    /* compute terms from cached_up_to+1 up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i <= 15) {
            cache[(size_t)i] = init[(size_t)i];
            continue;
        }

        seq_t val = cache[(size_t)(i - 1)] * 2
                  + cache[(size_t)(i - 3)] * 10
                  + cache[(size_t)(i - 5)] * 8
                  + cache[(size_t)(i - 9)] * 33
                  + cache[(size_t)(i -10)] * 4
                  + cache[(size_t)(i -15)];

        val -= cache[(size_t)(i - 4)] * 4;
        val -= cache[(size_t)(i - 6)] * 25;
        val -= cache[(size_t)(i - 8)] * 4;
        val -= cache[(size_t)(i -11)] * 4;
        val -= cache[(size_t)(i -12)] * 10;
        val -= cache[(size_t)(i -13)] * 2;

        cache[(size_t)i] = val;
    }

    cached_up_to = n;
    return cache[(size_t)n];
}
