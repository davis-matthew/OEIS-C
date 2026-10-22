/*
 * A358108  a(n) = 16^n * Sum_{k=0..n} binomial(-1/2, k)^2 * binomial(n, k).
 * https://oeis.org/A358108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A358108
   Formula: a(n) = 16^n * Σ_{k=0..n} binom(-1/2, k)^2 * binom(n, k)
   Equivalent integer form used below:
   a(n) = Σ_{k=0..n} 16^{n-k} * binom(2k, k)^2 * binom(n, k)
*/

#include <stdint.h>
#include "../utils/binomial.h"   /* binom(idx_t, idx_t) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t, idx_t) */

seq_t a(idx_t n) {
    /* 16^n as big integer */
    seq_t pow = seq_pow((seq_t)16, n);

    seq_t sum = 0;          /* running sum of terms */
    seq_t bin_nk = 1;       /* binom(n, k) with k = 0 */
    seq_t bin2   = 1;       /* binom(2k, k) with k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 16^{n-k} * binom(2k, k)^2 * binom(n, k) */
        seq_t term = pow;
        term = term * bin2;                  /* * binom(2k,k) */
        term = term * bin2;                  /* * binom(2k,k) again */
        term = term * bin_nk;                /* * binom(n,k)   */
        sum = sum + term;                    /* accumulate */

        /* prepare for next iteration if any */
        if (k == n) break;

        /* Update binom(n, k+1)  = binom(n,k) * (n-k)/(k+1)  */
        seq_t numer_nk = bin_nk * (seq_t)(n - k);
        seq_t denom_nk = (seq_t)(k + 1);
        bin_nk = numer_nk / denom_nk;

        /* Update binom(2(k+1), k+1)
           = binom(2k,k) * (2k+2)(2k+1)/(k+1)^2  */
        seq_t temp = bin2 * (seq_t)(2 * k + 2);
        temp = temp * (seq_t)(2 * k + 1);
        seq_t denom_bin2 = (seq_t)(k + 1) * (seq_t)(k + 1);
        bin2 = temp / denom_bin2;

        /* Reduce power factor from 16^{n-k} to 16^{n-(k+1)} */
        pow = pow / (seq_t)16;
    }

    return sum;
}
