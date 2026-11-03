/*
 * A400875  Array read by ascending antidiagonals: A(n,k) = numerator((n^2 -
 *          2*n*k - k^2)/(n^2 + k^2)), with k > 0.
 * https://oeis.org/A400875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac, tabl, new
 * Formula:   not stated
 * Range:     i8 n <= 64, i16 n <= 11325, i32 n <= 11325, i64 n <= 11325,
 *            i128 n <= 11325, big any n
 * Validated: standard, pass, 82 terms through n = 9170 (2026-11-02)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A400875: array read by ascending antidiagonals.
   A(n,k) = numerator((n^2 - 2*n*k - k^2)/(n^2 + k^2))  (k>0).
   The sequence a(n) is obtained by enumerating the array in the
   order (i,j) with i+j increasing, and for a fixed sum i descending
   from i = sum-1 to 1.
   For a given index n>=1 we first compute the diagonal sum S such
   that S(S-1)/2 >= n > (S-1)(S-2)/2, then the offset off = n-
   (S-1)(S-2)/2.  The mapped pair is i = S-off and j = off.
   The returned value is the reduced numerator of the fraction
   (i^2 - 2*i*j - j^2)/(i^2 + j^2).                             */

#include <stdint.h>
#include "../utils/gcd.h"

/* locate the diagonal sum S for a given 1‑based index n */
static idx_t
_diag_from_index(idx_t n)
{
    idx_t low = 1;
    idx_t high = 2;

    /* find an upper bound where high*(high-1)/2 >= n */
    while (1) {
        unsigned __int128 val = (unsigned __int128)high * (high - 1) / 2;
        if (val >= n)
            break;
        high <<= 1;
        if (high == 0) {          /* overflow guard, should never happen */
            high = (idx_t)-1;
            break;
        }
    }

    /* binary search between low+1 and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

seq_t
a(idx_t n)
{
    /* find diagonal and offset */
    idx_t S = _diag_from_index(n);
    unsigned __int128 prev = (unsigned __int128)(S - 1) * (S - 2) / 2;
    idx_t off = (idx_t)(n - prev);

    /* mapping to array coordinates */
    idx_t i = S - off;   /* row index n */
    idx_t j = off;       /* column index k */

    /* compute numerator and denominator as seq_t */
    seq_t i_t = (seq_t)i;
    seq_t j_t = (seq_t)j;

    seq_t num = i_t * i_t - 2 * i_t * j_t - j_t * j_t;
    seq_t den = i_t * i_t + j_t * j_t;

    seq_t abs_num = num >= 0 ? num : -num;
    seq_t g = gcd(abs_num, den);

    /* reduced numerator */
    return num / g;
}
