/*
 * A297426  Number of n X 3 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 7*a(n-3) + a(n-5) - 4*a(n-6) (Empirical
 *            recurrence from OEIS A297426) Initial values: a(1)=1, a(2)=7,
 *            a(3)=14, a(4)=22, a(5)=68, a(6)=163
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset */

    /* static cache:  cache[i] holds a(i) or 0 if not yet computed     */
    static seq_t *cache = NULL;
    static idx_t cap = 0;        /* number of allocated slots      */
    static idx_t max_computed = 0; /* highest index with value */

    /* enlarge cache if necessary (doubling strategy)                    */
    if (cap < n + 1) {
        idx_t newcap = cap ? cap << 1 : n + 1;
        if (newcap < n + 1) newcap = n + 1;

        seq_t *newbuf = alloc_seq_array(newcap);
        if (!newbuf) return 0;           /* allocation failed           */

        /* copy old values */
        if (cache) {
            memcpy(newbuf, cache, cap * sizeof(seq_t));
            free_seq_array(cache);
        }

        /* zero‑initialise new part (alloc_seq_array already produces
           default constructed elements, but we explicitly set 0 for safety) */
        memset(newbuf + cap, 0, (newcap - cap) * sizeof(seq_t));

        cache = newbuf;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1)          cache[i] = 1;
        else if (i == 2)     cache[i] = 7;
        else if (i == 3)     cache[i] = 14;
        else if (i == 4)     cache[i] = 22;
        else if (i == 5)     cache[i] = 68;
        else if (i == 6)     cache[i] = 163;
        else {
            /* recurrence uses already computed values */
            cache[i] = cache[i-1] + 7 * cache[i-3] + cache[i-5] - 4 * cache[i-6];
        }
    }
    max_computed = n;

    return cache[n];
}
