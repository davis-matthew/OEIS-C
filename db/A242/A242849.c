/*
 * A242849  Triangle read by rows: T(n,k) = A060828(n)/(A060828(k) *
 *          A060828(n-k)).
 * https://oeis.org/A242849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = largest power of 3 dividing the binomial coefficient
 *            C(r,c) where r = floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2
 *            (row and column read by rows)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   v3_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/v3_factorial.h"

seq_t a(idx_t n)
{
    /* Find row r such that the first element of row r is at index
       r*(r+1)/2 (0-based).  This satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = (idx_t)((sqrtl((long double)1 + (long double)8 * n) - 1) / 2);

    /* Correct possible rounding errors: adjust r until the inequality holds. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t prefix = r * (r + 1) / 2;   /* number of terms before row r */
    idx_t c = n - prefix;             /* column index within row r, 0 <= c <= r */

    /* Compute exponent of 3 in C(r,c) */
    idx_t e = v3_factorial(r) - v3_factorial(c) - v3_factorial(r - c);

    /* Return 3^e as seq_t.  If e==0, the result is 1. */
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i)
        result *= 3;

    return result;
}
