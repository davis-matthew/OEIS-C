/*
 * A299259  Coordination sequence for 3D uniform tiling formed by stacking
 *          parallel layers of the 4.8.8 2D tiling (cf. A008576).
 * https://oeis.org/A299259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5, a(2)=13, a(3)=26, a(4)=45, a(5)=69;
   a(n) = 2*a(n‑1) - a(n‑2) + a(n‑3) - 2*a(n‑4) + a(n‑5)  for n ≥ 6 */
#include <stdlib.h>

const seq_t INIT_VALUES[6] = { (seq_t)1, (seq_t)5, (seq_t)13, (seq_t)26,
                               (seq_t)45, (seq_t)69 };

seq_t a(idx_t n)
{
    if (n < 6)
        return INIT_VALUES[n];

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow cache (if necessary) to hold up to index n. */
    idx_t required = n + 1;
    if (required > cache_size) {
        /* Allocate or enlarge cache. */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)required * sizeof(seq_t));
        if (!tmp)
            abort();  /* fatal allocation failure */
        cache = tmp;

        /* Initialise base values up to index 5, if they are not already. */
        if (cache_size < 6) {
            for (idx_t i = cache_size; i < 6; ++i)
                cache[i] = INIT_VALUES[i];
            cache_size = 6;
        }

        /* Compute new terms using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t t5 = cache[i - 5];

            /* a(i) = 2*t1 - t2 + t3 - 2*t4 + t5 */
            seq_t res = t1 + t1;   /* 2*t1 */
            res -= t2;            /* -t2 */
            res += t3;            /* +t3 */
            res -= (t4 + t4);     /* -2*t4 */
            res += t5;            /* +t5 */
            cache[i] = res;
        }
        cache_size = required;
    }

    return cache[n];
}
