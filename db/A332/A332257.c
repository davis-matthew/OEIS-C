/*
 * A332257  E.g.f.: (1 - sinh(x)) / (1 - 2*sinh(x)).
 * https://oeis.org/A332257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, grow_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A332257 : a(0)=1,
    a(n) = 2 * Σ_{odd k=1..n} binomial(n,k) * a(n-k) - (n % 2)   (OEIS definition)
*/

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/grow_seq_array.h"

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* lazy static cache for memoisation */
    static seq_t *cache = NULL;          /* array of a values */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t  cache_max = (idx_t)(-1);/* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init = (size_t)n + 1;
        if (init == 0) init = 1;         /* safety for n = -1 (not used) */
        cache = seq_alloc_new_array(init);
        cache_cap = init;
        cache[0] = (seq_t)1;
        cache_max = 0;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
    }

    /* compute a[i] for all missing indices up to n */
    for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
        seq_t sum = 0;

        /* sum over odd k */
        for (idx_t k = 1; k <= cur; k += 2) {
            seq_t b = binomial(cur, k);          /* binomial coefficient */
            sum += b * cache[cur - k];
        }

        sum *= (seq_t)2;               /* multiply by 2 */

        if (cur & (seq_t)1) {          /* subtract 1 if cur is odd */
            sum -= 1;
        }

        cache[cur] = sum;
    }

    cache_max = n;
    return cache[n];
}
