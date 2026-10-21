/*
 * A254671  Numbers that can be represented as x*y + x + y, where x >= y > 1.
 * https://oeis.org/A254671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _has_ge3_factor_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254671: numbers k such that k+1 can be factored
   as a product of two integers ≥ 3.  The function returns the
   n‑th term (1‑based index).  It builds the sequence on demand
   and caches the values.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_has_ge3_factor_pair.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */

    static seq_t *vals   = NULL;       /* cached list of terms */
    static size_t  len   = 0;          /* number of stored terms   */
    static size_t  cap   = 0;          /* allocated capacity      */
    static uint64_t last  = 1;         /* last integer checked    */

    /* Allocate or grow the cache if needed. */
    if (cap == 0) {
        cap = 16;
        vals = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        ++last;
        if (last < 8) continue;        /* first term is 8 */

        /* Test whether last satisfies the condition. */
        if (_has_ge3_factor_pair((seq_t)(last + 1))) {
            if (len == cap) {
                cap <<= 1;
                vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
            }
            vals[len++] = (seq_t)last;
        }
    }

    return vals[n - 1];
}
