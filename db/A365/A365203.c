/*
 * A365203  a(1) = 1; a(n) = a(n - 1) + n if a(n - 1) < n, a(n) = n^2 if a(n -
 *          1) = n, a(n) = a(n - 1)/n if a(n - 1) > n and a(n - 1) == 0 (mod
 *          n), otherwise a(n) = a(n - 1) - n.
 * https://oeis.org/A365203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n) = a(n-1)+n      if a(n-1)<n
          = n*n          if a(n-1)==n
          = a(n-1)/n     if a(n-1)>n && a(n-1)%n==0
          = a(n-1)-n     otherwise
   Indexed starting at n=1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined; guard against 0 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed entries (max n) */

    /* first call: allocate space for a(1) */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        cache[0] = 1;                  /* a(1) = 1 */
        cache_len = 1;
    }

    /* extend the cache up to n if needed */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (new_cache == NULL) return 0;   /* allocation failure */
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];      /* a(i-1) */
            seq_t idx  = (seq_t)i;          /* i as seq_t */
            seq_t cur;

            if (prev < idx) {
                cur = prev + idx;
            } else if (prev == idx) {
                cur = idx * idx;
            } else if ((prev % idx) == 0) {
                cur = prev / idx;
            } else {
                cur = prev - idx;
            }

            cache[i - 1] = cur;   /* store a(i) */
        }

        cache_len = new_len;
    }

    return cache[n - 1];
}
