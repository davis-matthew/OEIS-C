/*
 * A250307  Number of 2-colored Schroeder paths of semilength n in which there
 *          are no (2,0)-steps at level 1.
 * https://oeis.org/A250307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=10, a(3)=37 and for n≥4 a(n) =
 *            ((3*(9*n-8)*a(n-1)+4*(43*n-81)*a(n-3)) -
 *            (4*(28*n-39)*a(n-2)+64*(n-3)*a(n-4))) / (2*n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

extern seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    size_t old_cap;

    /* ensure capacity for indices up to n */
    if (n + 1 > cache_cap) {
        old_cap = cache_cap;
        cache_cap = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (cache == NULL) abort();
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
        }
        /* initialise new elements */
        for (idx_t i = old_cap; i <= n; ++i) {
            seq_t val;
            if (i == 0)          val = (seq_t)1;
            else if (i == 1)     val = (seq_t)3;
            else if (i == 2)     val = (seq_t)10;
            else if (i == 3)     val = (seq_t)37;
            else {
                seq_t m1 = (seq_t)3 * ((seq_t)9 * i - (seq_t)8) * cache[i-1];
                seq_t m3 = (seq_t)4 * ((seq_t)43 * i - (seq_t)81) * cache[i-3];
                seq_t m2 = (seq_t)4 * ((seq_t)28 * i - (seq_t)39) * cache[i-2];
                seq_t m4 = (seq_t)64 * ((seq_t)(i-3)) * cache[i-4];
                seq_t numerator = (m1 + m3) - (m2 + m4);
                seq_t denom = (seq_t)2 * i;
                val = numerator / denom;
            }
            cache[i] = val;
        }
    }
    return cache[n];
}
