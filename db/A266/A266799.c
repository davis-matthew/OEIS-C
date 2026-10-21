/*
 * A266799  Coefficient of x^0 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5),
 *            with a(0)=1, a(1)=1, a(2)=49, a(3)=229, a(4)=1861
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t  cap   = 0;      /* current allocated size */
    static idx_t  size  = 0;      /* number of terms actually stored */

    /* initial terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)49,
                                   (seq_t)229, (seq_t)1861 };

    /* if the wanted term is already computed, return it */
    if (n < size)
        return cache[n];

    /* extend the cache if necessary */
    if (n >= cap || cache == NULL) {
        /* grow until capacity > n */
        idx_t newcap = (cap == 0) ? (n + 1) : (cap * 2);
        while (newcap <= n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed */
            return 0;
        cache = tmp;
        cap = newcap;
    }

    /* compute new terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 5)
            cache[i] = init[i];
        else
            cache[i] = (5 * cache[i-1] + 15 * cache[i-2]
                        - 15 * cache[i-3] - 5 * cache[i-4]
                        + cache[i-5]);
    }
    size = n + 1;

    return cache[n];
}
