/*
 * A365911  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(4*k+3) / (4*k+3)! ).
 * https://oeis.org/A365911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-3)/4)} binomial(n,4*k+3) * a(n-4*k-3)
 *            a(0)=1 , a(1)=0 , a(2)=0 , a(3)=1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 451 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* size_t */
#include <stdbool.h>    /* bool */
#include <stdlib.h>     /* malloc, realloc */
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;     /* number of allocated slots */
    static idx_t  max     = 0;     /* largest index already computed */

    /* initialise on first call */
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        if (!cache) return (seq_t)0;     /* allocation failed - unlikely */
        cache[0] = (seq_t)1;
        cap   = 1;
        max   = 0;
    }

    if (n > max) { /* need to extend cache up to n */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        for (idx_t i = max + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0;
                 (idx_t)(4 * k + 3) <= i;
                 ++k) {
                idx_t j = i - 4 * k - 3;
                seq_t comb = binomial(i, 4 * k + 3);
                seq_t sub  = cache[j];
                sum += comb * sub;
            }
            cache[i] = sum;
        }
        max = n;
    }

    return cache[n];
}
