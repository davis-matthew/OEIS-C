/*
 * A348712  Square array read by falling antidiagonals: T(n,k) is the number
 *          of bounded regions formed by the Lissajous curve x=cos(n*t),
 *          y=sin(k*t).
 * https://oeis.org/A348712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A348712:  Square array read by falling antidiagonals
 * For each n ≥ 1, determine the coordinates (i,j) of the n‑th
 * entry in the array T(i,j), where T(i,j) equals
 *
 *   (i-1)*(j-1) + i*j                if i is odd and gcd(i,j)=1,
 *   (i-1)*(j-1)/2                    if i is even and gcd(i,j)=1,
 *   T(i/gcd(i,j), j/gcd(i,j))        otherwise.
 *
 * The function returns that value in type seq_t.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal sum s = i + j such that
       total elements up to s  ≥ n.  Total up to s is s*(s-1)/2. */
    idx_t s = 2;
    for (; /* No operation */ ; ++s) {
        if (s * (s - 1) / 2 >= n) {
            break;
        }
    }

    /* Elements preceding this diagonal */ 
    idx_t prev_total = (s - 1) * (s - 2) / 2;
    /* Zero‑based offset inside the diagonal */
    idx_t offset = n - prev_total - 1;

    /* Coordinates in the array: i increases from 1 */
    idx_t i = offset + 1;
    idx_t j = s - i;

    /* Reduce by gcd to obtain coprime pair */
    seq_t a_val = (seq_t)i;
    seq_t b_val = (seq_t)j;
    seq_t g = gcd(a_val, b_val);
    if (g != 1) {
        a_val /= g;
        b_val /= g;
    }

    /* Compute the number of bounded regions */
    seq_t result;
    if ((a_val & 1) != 0) {                 /* a_val is odd   */
        result = (a_val - 1) * (b_val - 1) + a_val * b_val;
    } else {                                /* a_val is even  */
        result = ((a_val - 1) * (b_val - 1)) / 2;
    }

    return result;
}
