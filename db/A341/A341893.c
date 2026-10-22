/*
 * A341893  Indices of triangular numbers that are one-tenth of other
 *          triangular numbers.
 * https://oeis.org/A341893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 38*a(n‑3) – a(n‑6) + 18 (for n > 3), with a(1)=0, a(2)=1,
 *            a(3)=6, a(4)=12, a(5)=55
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* value not needed for the offset 1 sequence */

    /* memoised cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms stored */

    if (n > cache_size) {
        /* extend the cache */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t) n * sizeof(seq_t));
        if (!new_cache) abort();        /* memory allocation failure */
        cache = new_cache;

        /* compute all new terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)
                cache[0] = 0;
            else if (i == 2)
                cache[1] = 1;
            else if (i == 3)
                cache[2] = 6;
            else if (i == 4)
                cache[3] = 12;
            else if (i == 5)
                cache[4] = 55;
            else {
                seq_t pred3 = cache[i - 4];              /* a(n‑3) */
                seq_t pred6 = (i > 6) ? cache[i - 7] : 0;/* a(n‑6) or 0 when n‑6==0 */
                cache[i - 1] = 38 * pred3 - pred6 + 18;
            }
        }

        cache_size = n;
    }

    return cache[n - 1];
}
