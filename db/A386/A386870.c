/*
 * A386870  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+2,k) *
 *          binomial(4*n-k,n-k).
 * https://oeis.org/A386870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+2,k) *
 *            binomial(4*n-k,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a recurrence to evaluate the summation in O(n) time.  */
/* It handles both integral and arbitrary‑precision seq_t types.               */
/* For integral seq_t, 128‑bit temporaries are used to avoid overflow.         */

#ifndef A386870_H
#define A386870_H

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

#if defined(__cplusplus)
# include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* T_0 = 2^n * C(4n, n) */
    seq_t term = binomial(4 * n, n);
    term *= pow2(n);
    seq_t sum  = term;

    for (idx_t k = 1; k <= n; ++k) {
        /* Recurrence factor:
         *   T_k = T_{k-1} * (4n+3-k) * (n-k+1)
         *          ----------------------------------
         *                2 * k * (4n-k+1)              */
        idx_t a_coef = 4 * n + 3 - k;
        idx_t b_coef = n - k + 1;
        idx_t c_coef = 2 * k;
        idx_t d_coef = 4 * n - k + 1;

#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            /* Integral path – use 128‑bit temporaries */
            unsigned __int128 v = (unsigned __int128)term;   /* T_{k-1}      */
            v *= (unsigned __int128)a_coef;                 /* * (4n+3-k)   */
            v *= (unsigned __int128)b_coef;                 /* * (n-k+1)    */
            unsigned __int128 denom = (unsigned __int128)c_coef * d_coef;
            term = (seq_t)(v / denom);
        } else {
            /* Arbitrary‑precision path – all operations are exact */
            seq_t v = term;
            v *= a_coef;            /* * (4n+3-k) */
            v *= b_coef;            /* * (n-k+1) */
            seq_t denom = c_coef;
            denom *= d_coef;        /* 2*k*(4n-k+1) */
            term = v / denom;
        }
#else
        /* C (32/64‑bit) path – always use 128‑bit temporaries */
        unsigned __int128 v = (unsigned __int128)term;
        v *= (unsigned __int128)a_coef;
        v *= (unsigned __int128)b_coef;
        unsigned __int128 denom = (unsigned __int128)c_coef * d_coef;
        term = (seq_t)(v / denom);
#endif
        sum += term;
    }

    return sum;
}

#endif /* A386870_H */
