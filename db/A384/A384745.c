/*
 * A384745  Consecutive states of the linear congruential pseudo-random number
 *          generator (5^17*s+1) mod 2^48 when started at s=1.
 * https://oeis.org/A384745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^17 * a(n-1) + 1) mod 2^48, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n = 0, return 0 */

    static seq_t *cache = NULL;
    static idx_t cached = 0;       /* number of terms cached */

    static const seq_t MUL  = (seq_t)762939453125ULL;         /* 5^17 */
    static const seq_t MASK = ((seq_t)1 << 48) - 1;           /* mod 2^48 */

    if (cached < n) {
        /* allocate or grow cache */
        seq_t *new_cache = (seq_t*)malloc((size_t)n * sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */

        /* copy existing values */
        for (idx_t i = 0; i < cached; ++i) new_cache[i] = cache[i];
        if (cache) free(cache);
        cache = new_cache;

        /* initialize first term if the cache was empty */
        if (cached == 0) {
            cache[0] = (seq_t)1;
            cached = 1;
        }

        /* compute missing terms */
        seq_t last = cache[cached - 1];
        for (idx_t i = cached; i < n; ++i) {
            seq_t next = ((last * MUL) + 1) & MASK;
            cache[i] = next;
            last = next;
        }
        cached = n;
    }

    return cache[n - 1];
}
