/*
 * A278405  a(n) = Sum_{k=0..n} binomial(n,2k)^2*binomial(n-k,k).
 * https://oeis.org/A278405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,2k)^2 * binomial(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Result accumulator */
    seq_t sum = 0;

    /* B_k holds binomial(n,2k), starting with k=0 (value 1) */
    seq_t B_k = 1;
    /* C_k holds binomial(n-k,k), starting with k=0 (value 1) */
    seq_t C_k = 1;

    /* Upper bound for k */
    idx_t k, maxk = n / 2;
    for (k = 0; k <= maxk; ++k)
    {
        /* Add current term: B_k^2 * C_k */
        seq_t term = B_k * B_k;
        term *= C_k;
        sum += term;

        /* No need to compute next values after last iteration */
        if (k == maxk)
            break;

        /* ---- Compute B_{k+1} ---- */
        /* B_{k+1} = B_k * (n-2k) / (2k+1) * (n-2k-1) / (2k+2) */
        seq_t t = B_k * (seq_t)(n - 2 * k);
        t = t / (seq_t)(2 * k + 1);
        t = t * (seq_t)(n - 2 * k - 1);
        t = t / (seq_t)(2 * k + 2);
        B_k = t;

        /* ---- Compute C_{k+1} ---- */
        /* C_{k+1} = C_k * (n-2k) / (n-k) * (n-2k-1) / (k+1) */
        t = C_k * (seq_t)(n - 2 * k);
        t = t / (seq_t)(n - k);
        t = t * (seq_t)(n - 2 * k - 1);
        t = t / (seq_t)(k + 1);
        C_k = t;
    }

    return sum;
}
