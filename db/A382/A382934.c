/*
 * A382934  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) *
 *          binomial(n+2*k,k) * 2^(n-k).
 * https://oeis.org/A382934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n}
 *            binomial(n,k)*binomial(n+k,k)*binomial(n+2*k,k)*2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Each term is
         term_k = C(n,k)*C(n+k,k)*C(n+2k,k)*2^(n-k)
    */
    seq_t term = pow2(n);          /* k = 0 term */
    seq_t sum  = term;             /* start the sum */

    /* For C++ builds (seq_t may be arbitrary precision) we can use the
       straightforward recurrence with no overflow.  For pure C builds
       (seq_t is 64‑bit) use 128‑bit intermediates to avoid overflow. */
#ifdef __cplusplus
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Multiply by numerator factors */
        term = term * (n - k + 1);
        term = term * (n + 2 * k - 1);
        term = term * (n + 2 * k);

        /* Divide by denominator (k^3 * 2) */
        term = term / ((seq_t)k * (seq_t)k * (seq_t)k * 2);

        sum += term;
    }
#else
    for (idx_t k = 1; k <= n; ++k)
    {
        unsigned __int128 temp = (unsigned __int128)term;

        temp *= (unsigned __int128)(n - k + 1);
        temp *= (unsigned __int128)(n + 2 * k - 1);
        temp *= (unsigned __int128)(n + 2 * k);

        temp /= (unsigned __int128)(k);
        temp /= (unsigned __int128)(k);
        temp /= (unsigned __int128)(k);
        temp /= (unsigned __int128)2;

        term = (seq_t)temp;
        sum  = sum + term;
    }
#endif

    return sum;
}
