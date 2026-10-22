/*
 * A339710  a(n) = Sum_{k=0..n} binomial(n, k)*binomial(2*n + k, k)*2^k.
 * https://oeis.org/A339710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k) * binomial(2*n + k, k) * 2^k
 *            This implementation iteratively builds the sum to avoid large
 *            intermediate values. For builds where seq_t is a native integer
 *            type of at most 64 bits an intermediate __int128 type is used to
 *            keep the computation exact; for arbitrary‑...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the generic case with arbitrary precision arithmetic.  */
    if (sizeof(seq_t) > 8) {
        seq_t term = 1;
        seq_t sum  = 1;
        for (idx_t k = 1; k <= n; ++k) {
            term = term * (seq_t)(n - k + 1);
            term = term * (seq_t)(2 * n + k);
            term = term * (seq_t)2;
            term = term / ((seq_t)k * (seq_t)k);
            sum  = sum + term;
        }
        return sum;
    }

    /* For 64‑bit (or smaller) types use a __int128 intermediate to avoid
       overflow before the final division.  It is assumed that the caller
       only requests values that fit in the native seq_t.  */
    {
        __int128 term = 1;
        __int128 sum  = 1;
        for (idx_t k = 1; k <= n; ++k) {
            term = term * (n - k + 1);
            term = term * (2 * n + k);
            term = term * 2;
            __int128 denom = (__int128)k * k;
            term = term / denom;           /* exact division */
            sum  += term;
        }
        return (seq_t)sum;
    }
}
