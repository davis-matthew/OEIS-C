/*
 * A373160  a(n) = denominator of Sum_{k>=0} cos(k*Pi/3)/n^k.
 * https://oeis.org/A373160
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = denominator of
 *              ∑_{k≥0} cos(k·π/3) / n^k
 * Derived closed‑form:  a(n) = (2n(n^2−n+1)) / gcd( (2n−1)n^2 , 2n(n^2−n+1) )
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 2.  For other values the
       behaviour is irrelevant. */
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n * (seq_t)n;                     /* n²                     */
    seq_t num = ((seq_t)2 * (seq_t)n - (seq_t)1) * nn;  /* (2n−1)·n²              */
    seq_t denom_factor = nn - (seq_t)n + (seq_t)1;       /* n²−n+1                 */
    seq_t den = (seq_t)2 * (seq_t)n * denom_factor;     /* 2n·(n²−n+1)            */

    seq_t g = gcd(num, den);
    return den / g;
}
