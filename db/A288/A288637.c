/*
 * A288637  Number of cycles in the grid graph P_4 X P_{n+1}.
 * https://oeis.org/A288637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 9*a(n-1)-27*a(n-2)+38*a(n-3)-29*a(n-4)+11*a(n-5)+a(n-6)-2*a(n-7)  (n>7)
   initial values: a(1)=6, a(2)=40, a(3)=213, a(4)=1049, a(5)=5034, a(6)=23984, a(7)=114069
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache   = NULL;  /* array of computed a(k) values */
    static idx_t cached_last = 0;  /* largest n already computed */
    static idx_t cache_cap   = 0;  /* current capacity of cache */

    /* minimal values for 1..7 */
    static const seq_t init[8] = {
        0ULL,
        6ULL,     40ULL,     213ULL,     1049ULL,
        5034ULL,  23984ULL,  114069ULL
    };

    if (n == 0) return 0ULL;  /* offset is 1 */

    /* allocate cache if needed */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0ULL;  /* allocation failure */
        cache[0] = 0ULL;          /* unused, but keeps index alignment */
    }

    /* grow cache until it can hold index n */
    while (cache_cap <= n) {
        idx_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0ULL;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = cached_last + 1; k <= n; ++k) {
        if (k <= 7) {
            cache[k] = init[k];
        } else {
            /* compute positive and negative parts separately to avoid
               undefined behaviour with unsigned subtraction when a(n-1)
               ... < a(n-2), etc.  All products fit in seq_t for the
               usable range of a(n). */
            seq_t pos = 9   * cache[k-1] +
                         38  * cache[k-3] +
                         11  * cache[k-5] +
                         1   * cache[k-6];
            seq_t neg = 27  * cache[k-2] +
                         29  * cache[k-4] +
                         2   * cache[k-7];
            cache[k] = pos - neg;
        }
    }

    cached_last = n;
    return cache[n];
}
