/*
 * A393378  a(n) = (8*n)! / ((3*n)! * (n!)^5).
 * https://oeis.org/A393378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n)! / ((3*n)! * (n!)^5)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; a(n) satisfies the recurrence:
   a(n) = 128*(8n-5)*(4n-1)*(8n-7)*(2n-1)*(8n-1)*(4n-3)*(8n-3)*a(n-1)
          -------------------------------------------,
          3*n^5*(3n-1)*(3n-2) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to allow unordered calls */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of cache array */
    static idx_t cache_len = 0;   /* number of values stored (last index + 1) */

    /* Ensure cache exists and contains a(0) */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely in tests */
        cache[0] = 1;                  /* a(0) = 1 */
        cache_len = 1;
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Expand cache capacity if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n**
       recurrence: a(0)=1, for i>=1
       a(i) = num / den
          num = 128 * (8*i-5) * (4*i-1) * (8*i-7) * (2*i-1) *
                (8*i-1) * (4*i-3) * (8*i-3) * a(i-1)
          den = 3 * i^5 * (3*i-1) * (3*i-2)    */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* numerator */
        seq_t num = cache[i - 1];
        num *= (seq_t)128;
        num *= (seq_t)(8 * i - 5);
        num *= (seq_t)(4 * i - 1);
        num *= (seq_t)(8 * i - 7);
        num *= (seq_t)(2 * i - 1);
        num *= (seq_t)(8 * i - 1);
        num *= (seq_t)(4 * i - 3);
        num *= (seq_t)(8 * i - 3);

        /* denominator */
        seq_t den = (seq_t)3;
        den *= (seq_t)i;
        den *= (seq_t)i;
        den *= (seq_t)i;
        den *= (seq_t)i;
        den *= (seq_t)i;                 /* i^5 */
        den *= (seq_t)(3 * i - 1);
        den *= (seq_t)(3 * i - 2);

        cache[i] = num / den;            /* integer division is exact */
    }

    cache_len = n + 1;   /* we now have values up to n */
    return cache[n];
}
