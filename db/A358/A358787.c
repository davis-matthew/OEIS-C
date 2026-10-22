/*
 * A358787  a(1)=1; let x=gcd(a(n-1),n); for n > 1, a(n) = a(n-1) + n if x=1
 *          or a(n-1)/x=1, otherwise a(n) = a(n-1)/x.
 * https://oeis.org/A358787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1 let g=gcd(a(n-1),n);
   if g==1 or a(n-1)/g==1 then a(n)=a(n-1)+n else a(n)=a(n-1)/g */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values (0-based indices) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t cur_max = 0;   /* number of terms stored, >=0 */

    if (n == 0) return 0;          /* offset starts at 1 */

    /* return cached value if already computed */
    if (n <= cur_max)
        return cache[n - 1];

    /* initialise cache with a(1)=1 if first call */
    if (cur_max == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cur_max = 1;
    }

    /* grow cache if necessary */
    if (cap < n) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t prev = cache[cur_max - 1];
        seq_t g    = gcd(prev, (seq_t)i);
        seq_t next;
        if (g == (seq_t)1 || prev / g == (seq_t)1)
            next = prev + (seq_t)i;
        else
            next = prev / g;
        cache[cur_max] = next;   /* store at index cur_max (0-based) */
        ++cur_max;
    }

    return cache[n - 1];
}
