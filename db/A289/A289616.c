/*
 * A289616  A246604 (Catalan(n)-n) with initial terms 1,0,0,2,10 changed to
 *          1,1,1,2,11.
 * https://oeis.org/A289616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = catalan(n) – n, with special initial values * a(0) = 1,
 *            a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 11
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* small indices need special handling */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)1;
    if (n == 3u) return (seq_t)2;
    if (n == 4u) return (seq_t)11;

    /* for n >= 5 the sequence follows the simple formula */
    seq_t cat = catalan(n);
    return cat - (seq_t)n;
}
