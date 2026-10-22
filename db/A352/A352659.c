/*
 * A352659  a(n) = n! * Sum_{k=0..floor(n/3)} 1 / (3*k)!.
 * https://oeis.org/A352659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} 1/(3k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:                                                  */
/*   a(0)=1, a(1)=1, a(2)=2                                    */
/*   for n>=3:                                                   */
/*     if n%3==0 : a[n] = n * a[n-1] + 1                        */
/*     if n%3==1 : a[n] = n * a[n-1]                            */
/*     if n%3==2 : a[n] = n * (n-1) * a[n-2]                    */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated size of cache   */
    static idx_t  last_computed = 0; /* highest index fully computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 3;                      /* store a[0], a[1], a[2] */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last_computed = 2;
    }

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i % 3 == 0) {
                cache[i] = (seq_t)i * cache[i - 1] + 1;
            } else if (i % 3 == 1) {
                cache[i] = (seq_t)i * cache[i - 1];
            } else { /* i % 3 == 2 */
                cache[i] = (seq_t)i * (seq_t)(i - 1) * cache[i - 2];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
