/*
 * A358368  a(n) = Sum_{k=0..n} C(n)^2 * binomial(n + k, k), where C(n) is the
 *          n-th Catalan number.
 * https://oeis.org/A358368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n + 1) * C(n)^3 (C(n) = n-th Catalan number)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided helper: seq_t catalan(idx_t k); */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat   = catalan(n);           /* C(n) */
    seq_t cat2  = cat * cat;            /* C(n)^2 */
    seq_t cat3  = cat2 * cat;           /* C(n)^3 */
    seq_t mult  = (seq_t)(2*n + 1);     /* (2*n + 1) */
    return mult * cat3;                 /* (2*n + 1) * C(n)^3 */
}
