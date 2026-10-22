/*
 * A358436  a(n) = Sum_{j=0..n} C(n)*C(n-j), where C(n) is the n-th Catalan
 *          number.
 * https://oeis.org/A358436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) * Σ_{k=0..n} C(k) (C = Catalan number)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                     /* malloc, realloc */
#include "../utils/catalan.h"           /* seq_t catalan(idx_t) */

/* Returns a(n) for the OEIS sequence A358436.
   The function is memoised so repeated calls for arbitrary n are fast. */
seq_t a(idx_t n)
{
    /* static cache of cumulative sums S[k] = Σ_{i=0..k} C(i) */
    static seq_t *cum      = NULL;      /* array of cumulative sums   */
    static idx_t  last    = 0;           /* highest index cached       */
    static size_t cap     = 0;           /* allocated capacity         */
    static int    init    = 0;           /* zero on first call         */

    /* ensure capacity for indices up to n */
    if (n >= cap) {
        size_t new_cap = (size_t)(n + 1);      /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                     /* allocation failed → 0 */
        cum = tmp;
        cap = new_cap;
    }

    /* initialisation for the first call */
    if (!init) {
        cum[0] = catalan(0);   /* C(0) = 1 */
        last = 0;
        init = 1;
    }

    /* extend the cache to n, if needed */
    while (last < n) {
        idx_t next = last + 1;
        seq_t c = catalan(next);
        cum[(size_t)next] = cum[(size_t)last] + c;
        last = next;
    }

    /* a(n) = C(n) * S(n)  where S(n) = cum[n] */
    seq_t cn = catalan(n);
    return cn * cum[(size_t)n];
}
