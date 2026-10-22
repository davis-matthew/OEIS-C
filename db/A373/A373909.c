/*
 * A373909  Number of compositions of 7*n into parts 3 and 7.
 * https://oeis.org/A373909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 36*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7), initial values: a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=9, a(5)=37, a(6)=122
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* base values for n = 0..6 */
    static const seq_t base[7] = {1, 1, 1, 2, 9, 37, 122};

    /* cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 6;          /* highest index already cached */

    if (n < 7)                              /* return pre‑computed value */
        return base[n];

    /* initialise cache with base values on first use */
    if (cache == NULL) {
        cache_cap = 8;                      /* at least 8 elements */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;               /* allocation failure fallback */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = base[i];
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;                  /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = 7 * cache[i - 1];           /* +7*a(i-1)   */
        pos += 36 * cache[i - 3];              /* +36*a(i-3)  */
        pos += 21 * cache[i - 5];              /* +21*a(i-5)  */
        pos += cache[i - 7];                   /* +a(i-7)     */

        seq_t neg = 21 * cache[i - 2];          /* +21*a(i-2)  */
        neg += 35 * cache[i - 4];              /* +35*a(i-4)  */
        neg += 7  * cache[i - 6];              /* +7*a(i-6)   */

        /* all terms in pos are ≥ all terms in neg for i ≥ 7,
           so subtraction is safe for unsigned seq_t types */
        cache[i] = pos - neg;
    }

    max_computed = n;
    return cache[n];
}
