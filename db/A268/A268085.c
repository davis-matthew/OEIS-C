/*
 * A268085  a(n) = Catalan(n)^2*n.
 * https://oeis.org/A268085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n)^2 * n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);           /* Catalan(n) */
    seq_t res = cat * cat;            /* Catalan(n)^2   */
    res = res * (seq_t)n;             /* * n            */
    return res;
}
