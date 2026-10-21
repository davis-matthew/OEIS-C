/*
 * A251012  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements less than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 18*a(n-2) + 18*a(n-3) - 7*a(n-4) + a(n-5),
 *            with initial values a(1)=63, a(2)=423, a(3)=2828, a(4)=18910,
 *            a(5)=126468
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache of already computed terms, 1‑based index. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static const seq_t init[5] = {63, 423, 2828, 18910, 126468};

    if (n == 0) return 0;                /* undefined for n<1, safe guard */

    /* Extend cache to length n, recomputing missing values. */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (new_len + 1) * sizeof(seq_t)); /* +1 for 1‑based */
        if (!tmp) return 0;                           /* allocation failure */
        cache = tmp;

        /* Compute terms from cache_len+1 up to new_len. */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            if (i <= 5) {                     /* initialise known terms */
                cache[i] = init[i - 1];
            } else {                           /* recurrence for i>5 */
                seq_t v = 9 * cache[i - 1]
                        - 18 * cache[i - 2]
                        + 18 * cache[i - 3]
                        - 7 * cache[i - 4]
                        + cache[i - 5];
                cache[i] = v;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
