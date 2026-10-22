/*
 * A367245  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 4.
 * https://oeis.org/A367245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 12*a(n-1) - 47*a(n-2) + 60*a(n-3) for n > 3; a(1)=0,
 *            a(2)=11, a(3)=135. This implementation uses a recurrence with
 *            memoisation to provide O(n) time and O(n) memory for all
 *            computed terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cap   = 0;       /* current capacity of cache  */
    static idx_t  last  = 0;       /* highest index already computed */

    if (n == 0)          /* out of offset – just return 0 */
        return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 4;                            /* indices 0..3 are needed */
        cache = seq_alloc(cap);               /* zero‑initialised array   */
        cache[1] = 0;
        cache[2] = 11;
        cache[3] = 135;
        last  = 3;
    }

    /* grow cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until large enough */
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* extend computation up to n if not already done */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = 12 * cache[i-1] - 47 * cache[i-2] + 60 * cache[i-3];
        }
        last = n;
    }

    return cache[n];
}
