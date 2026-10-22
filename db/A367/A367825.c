/*
 * A367825  Array read by ascending antidiagonals: A(n, k) is the denominator
 *          of (R(n) - k)/(n + k), where R(n) is the digit reversal of n, with
 *          A(0, 0) = 1.
 * https://oeis.org/A367825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8178, u16 n <= 9685, u32 n <= 9685, u64 n <= 9685,
 *            u128 n <= 9685, big any n
 * Helpers:   gcd, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A367825: Array read by ascending antidiagonals.
   A(n, k) = denominator of (R(n) - k)/(n + k)
   The sequence a(n) is obtained by reading all A(i, j)
   in order of increasing i + j, starting at (0,0).
   The function below implements that definition.
*/

#include <stdint.h>
#include "../utils/rev.h"   /* idx_t rev(idx_t) */
#include "../utils/gcd.h"   /* seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n) {
    /* A(0,0) is defined to be 1 */
    if (n == 0) return (seq_t)1;

    /* Find the diagonal number s such that
       t(s) = s*(s+1)/2 <= n < t(s+1)           */
    idx_t low = 0, high = 1;
    while (((seq_t)high * (high + 1)) / 2 <= n) {
        high <<= 1;                /* exponential search for an upper bound */
    }
    while (low < high) {          /* binary search between low and high-1   */
        idx_t mid = (low + high + 1) >> 1;
        if (((seq_t)mid * (mid + 1)) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t s = low;                /* diagonal index                         */

    /* triangular number t(s) fit in idx_t because t(s) <= n < 2^64 */
    idx_t tri = (idx_t)(((seq_t)s * (s + 1)) / 2);

    idx_t d = n - tri;            /* distance inside the diagonal          */

    /* Coordinates (i, j) of the element in the array
       where i is the row index, j is the column index (k)   */
    idx_t j = d;
    idx_t i = s - j;

    /* Compute the denominator of (R(i) - j)/(i + j)          */
    seq_t rev_i  = (seq_t)rev(i);                 /* R(i) */
    seq_t num    = rev_i >= j ? rev_i - (seq_t)j
                              : (seq_t)j - rev_i;  /* |R(i) - j|   */
    seq_t denom  = (seq_t)(i + j);

    /* Simplify by gcd                                               */
    seq_t g = gcd(num, denom);

    return denom / g;
}
