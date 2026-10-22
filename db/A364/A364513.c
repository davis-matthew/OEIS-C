/*
 * A364513  Square array read by ascending antidiagonals: T(n,k) = [x^k] (1 -
 *          x)^(2*k) * Legendre_P(n*k-1, (1 + x)/(1 - x)) for n, k >= 0.
 * https://oeis.org/A364513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the anti‑diagonal flattening of the square array *
 *            T(n,k) = Σ_{i=0..k} C(n*k−1, k−i)^2 * C((n−2)*k + i − 2, i) *
 *            with the special cases T(n,0)=1 and * T(0,k)= (−1)^k * C(2k,k)
 *            (k>0)
 * Range:     i8 n <= 13, i16 n <= 30, i32 n <= 59, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"

/* Generalized binomial for possibly negative upper index.
   For a < 0, C(a, b) = (−1)^b * C(−a + b − 1, b).            */
static inline seq_t binom_general(long long a, idx_t b)
{
    if (b == 0) return (seq_t)1;
    if (a >= 0) return binom((idx_t)a, b);
    /* a < 0 */
    long long m = -a;                     /* m > 0 */
    seq_t val = binom((idx_t)(m + b - 1), b);
    if ((b & 1) == 0) return val;
    else              return -val;
}

/* a(n) ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the anti‑diagonal index d such that
       d*(d+1)/2 <= n < (d+1)*(d+2)/2.               */
    idx_t d = 0;
    while (true) {
        idx_t next = d + 1;
        unsigned __int128 tri = (unsigned __int128)next * (next + 1) / 2;
        if (tri > n) break;
        d = next;
    }

    /* k along the diagonal, n_val = d − k */
    idx_t prev  = d * (d + 1) / 2;
    idx_t k     = n - prev;
    idx_t n_val = d - k;

    /* Compute T(n_val, k) ------------------------------------- */
    seq_t result = 0;

    if (k == 0) {
        /* T(n,0) = 1 for all n */
        result = (seq_t)1;
    } else if (n_val == 0) {
        /* T(0,k) = (−1)^k * C(2k,k)  */
        seq_t sign = ((k & 1) ? -(seq_t)1 : (seq_t)1);
        result = sign * binom(2 * k, k);
    } else {
        /* Sum formula for n >= 1 and k >= 1 */
        idx_t nk_minus1 = n_val * k - 1;
        seq_t sum = 0;
        for (idx_t i = 0; i <= k; ++i) {
            idx_t ki = k - i;                     /* k−i  */
            seq_t term1 = binom(nk_minus1, ki);
            seq_t term2 = binom_general((long long)((n_val - 2) * (long long)k + (long long)i - 2), i);
            seq_t term = term1 * term1;
            term *= term2;
            sum += term;
        }
        result = sum;
    }

    return result;
}
