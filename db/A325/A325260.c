/*
 * A325260  Number of integer partitions of n whose omega-sequence covers an
 *          initial interval of positive integers.
 * https://oeis.org/A325260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-5) - a(n-6) - a(n-7) +
 *            a(n-9) (n > 9) with initial values a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=2, a(4)=4, a(5)=5, a(6)=5, a(7)=8, a(8)=10, a(9)=12
 * Range:     u8 n <= 44, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;   /* points to cached values */
    static size_t cap   = 0;      /* allocated size of cache   */
    static size_t used  = 0;      /* number of values already stored (0 … used-1) */

    /* initial data for 0 … 9 */
    static const seq_t init[10] = {1, 1, 2, 2, 4, 5, 5, 8, 10, 12};

    /* make sure the cache can hold index n */
    size_t idx = (size_t)n;
    if (idx >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap <= idx) new_cap = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing values up to idx */
    if (used <= idx) {
        for (size_t i = used; i <= idx; ++i) {
            if (i < 10) {
                cache[i] = init[(int)i];
            } else {
                /* recurrence a(n) = a(n-2) + a(n-3) + a(n-4)
                                 - a(n-5) - a(n-6) - a(n-7) + a(n-9) */
                seq_t sum1 = cache[i-2] + cache[i-3] + cache[i-4];
                seq_t sum2 = cache[i-5] + cache[i-6] + cache[i-7];
                cache[i]   = sum1 - sum2 + cache[i-9];
            }
        }
        used = idx + 1;
    }

    return cache[idx];
}
