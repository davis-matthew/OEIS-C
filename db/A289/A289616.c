/*
 * A289616  A246604 (Catalan(n)-n) with initial terms 1,0,0,2,10 changed to
 *          1,1,1,2,11.
 * https://oeis.org/A289616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = catalan(n) – n, with special initial values * a(0) = 1,
 *            a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 11
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
