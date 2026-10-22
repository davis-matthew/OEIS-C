/*
 * A347277  Table T(n,k) read by downward antidiagonals: A quotient belonging
 *          to a generalization of Euler's theorem.
 * https://oeis.org/A347277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k^n - k^(n-phi(n))) / n read by downward antidiagonals
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find t such that t*(t+1)/2 >= n (t is the diagonal index).  */
    idx_t low = 0, high = 1;
    /* Increase high until it suffices.  */
    while ( ((__uint128_t)high * (high + 1)) / 2 < n ) {
        high <<= 1;
        if (high == 0) { high = (idx_t)-1; break; } /* overflow guard */
    }
    /* Binary search between low and high.  */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = ((__uint128_t)mid * (mid + 1)) / 2;
        if (tri >= n) high = mid;
        else low = mid;
    }
    idx_t t = high;                 /* diagonal number (t >= 1) */

    /* Number of entries before this diagonal.  */
    __uint128_t prevTri = ((__uint128_t)t * (t - 1)) / 2;
    idx_t offset = n - (idx_t)prevTri - 1;   /* 0‑based position on diagonal */

    idx_t row = offset + 1;          /* n in the definition */
    idx_t sum  = t + 1;              /* row + col */
    idx_t col  = sum - row;          /* k in the definition */

    /* Compute phi(row) and the required exponent.  */
    seq_t phi_val = phi(row);        /* phi(row) fits in seq_t */
    idx_t phi_int = (idx_t)phi_val;  /* small enough for idx_t */

    seq_t base = (seq_t)col;

    seq_t pow1 = seq_pow(base, row);
    seq_t pow2 = seq_pow(base, row - phi_int);

    seq_t diff = pow1 - pow2;
    seq_t result = diff / (seq_t)row;

    return result;
}
