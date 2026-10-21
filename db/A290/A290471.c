/*
 * A290471  Number of minimal edge covers in the n-wheel graph.
 * https://oeis.org/A290471
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+3*a(n-2)+a(n-3)-5*a(n-4)-5*a(n-5)+4*a(n-7)+3*a(n-8)+a(n-9)
 *            for n>10, with a(2)=1, a(3)=5, a(4)=7, a(5)=17, a(6)=31,
 *            a(7)=52, a(8)=99, a(9)=169, a(10)=301. The sequence is computed
 *            by dynamic programming and memoised between calls
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"  /* alloc_seq_array(size_t) -> seq_t*   */
#include "../utils/grow_seq_array.h"   /* grow_seq_array(seq_t**,size_t*,size_t) */

seq_t a(idx_t n)
{
    /* Base values for a(2) .. a(10) */
    static const seq_t base[9] = {1,5,7,17,31,52,99,169,301};

    /* Memoised cache */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static size_t  cap   = 0;        /* allocated capacity (elements) */
    static idx_t  last   = 0;        /* highest index already computed */

    /* For indices below the offset (n < 2) the behaviour is unspecified.
       Returning 0 is safe as the OEIS offset is 2. */
    if (n < 2) return (seq_t)0;

    /* Initialise cache on first call */
    if (!cache) {
        cap = (n >= 10) ? (size_t)(n + 1) : 11;          /* at least 11 to hold a(10) */
        cache = alloc_seq_array(cap);
        /* zero out the array – ensures defined values for indices < 2 */
        for (size_t i = 0; i < cap; ++i) cache[i] = (seq_t)0;
        /* initialise known a(2) … a(10) */
        for (idx_t i = 2; i <= 10 && (size_t)i < cap; ++i)
            cache[i] = base[i - 2];
        last = (cap > 10) ? 10 : (idx_t)(cap - 1);
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cap) {
        grow_seq_array(&cache, &cap, (size_t)n + 1);
        /* zero the new part of the array – only indices > last need clearing */
        for (size_t i = last + 1; i < cap; ++i) cache[i] = (seq_t)0;
    }

    /* Return immediately if already computed */
    if (n <= last) return cache[n];

    /* Compute new values up to n using the recurrence.
       We perform all additions before any subtractions to avoid
       under‑flow on unsigned types. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* term calculation: all additions first */
        seq_t t = cache[i - 1];
        t += 3 * cache[i - 2];
        t += cache[i - 3];
        t += 4 * cache[i - 7];
        t += 3 * cache[i - 8];
        t += cache[i - 9];
        /* subtractions */
        t -= 5 * cache[i - 4];
        t -= 5 * cache[i - 5];
        cache[i] = t;
    }
    last = n;
    return cache[n];
}
