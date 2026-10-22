/*
 * A303930  Number of no-leaf subgraphs of the 2 X n grid up to horizontal and
 *          vertical reflection.
 * https://oeis.org/A303930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=10, a(5)=26, a(6)=76, a(7)=232,
 *            a(8)=750 For n>8: a(n) = 8*a(n-1) - 16*a(n-2) - 20*a(n-3) +
 *            95*a(n-4) - 60*a(n-5) - 80*a(n-6) + 100*a(n-7) - 25*a(n-8)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* one‑based cache: cache[i-1] = a(i) */
    static size_t cache_len = 0;   /* number of stored terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* n must be >=1 – calls with n==0 are ignored */
    if (n == 0) return 0;

    /* initialise cache with the first 8 terms once */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as error value */

        /* seed the first 8 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)26;
        cache[5] = (seq_t)76;
        cache[6] = (seq_t)232;
        cache[7] = (seq_t)750;
        cache_len = 8;
    }

    /* if the value is already computed, return it */
    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* grow cache up to the requested index */
    while (cache_len < (size_t)n) {
        /* grow underlying storage if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – free cache and return 0 */
                free(cache);
                cache = NULL;
                cache_len = 0;
                cache_cap = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute next term using the recurrence */
        size_t i = cache_len + 1;              /* term index to compute (1‑based) */

        seq_t term =
              (seq_t)8   * cache[i-2]
            - (seq_t)16  * cache[i-3]
            - (seq_t)20  * cache[i-4]
            + (seq_t)95  * cache[i-5]
            - (seq_t)60  * cache[i-6]
            - (seq_t)80  * cache[i-7]
            + (seq_t)100 * cache[i-8]
            - (seq_t)25  * cache[i-9];

        cache[cache_len] = term;    /* store a(i) */
        cache_len++;
    }

    return cache[n-1];
}
