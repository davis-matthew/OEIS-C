/*
 * A254671  Numbers that can be represented as x*y + x + y, where x >= y > 1.
 * https://oeis.org/A254671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9660 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Helper: test whether v (≥ 3) can be written as a*b with a≥3, b≥3. */
static bool _has_ge3_factor_pair(seq_t v)
{
    /* v is assumed to fit in a 64‑bit unsigned integer for the
       ranges we need (< 2·10⁴), so cast safely. */
    uint64_t vv = (uint64_t)v;
    for (uint64_t d = 3; d * d <= vv; ++d) {
        if (vv % d == 0) {
            uint64_t e = vv / d;
            if (e >= 3) return true;
        }
    }
    return false;
}

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
        vals = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        ++last;
        if (last < 8) continue;        /* first term is 8 */

        /* Test whether last satisfies the condition. */
        if (_has_ge3_factor_pair((seq_t)(last + 1))) {
            if (len == cap) {
                cap <<= 1;
                vals = mem_resize(seq_t, vals, cap);
            }
            vals[len++] = (seq_t)last;
        }
    }

    return vals[n - 1];
}
