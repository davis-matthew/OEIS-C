/*
 * A366203  a(n) = (1/n) * Sum_{k=0..n-1} binomial(n+k-1,k) *
 *          binomial(3*n,n-k-1) * (n-3)^k.
 * https://oeis.org/A366203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} binomial(n+k-1,k) *
 *            binomial(3*n,n-k-1) * (n-3)^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Handle the two small indices explicitly to avoid negative powers */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* n >= 3: all powers are non‑negative */
    const seq_t base = (seq_t)n - 3;               /* (n-3) >= 0   */
    const seq_t three_n = (seq_t)n * 3;           /* 3*n          */

    seq_t pow = 1;                                 /* (n-3)^k      */
    seq_t bin1 = 1;                                /* binomial(n-1,0) */
    seq_t bin2 = binomial(3 * n, n - 1);           /* binomial(3n,n-1) */
    seq_t sum = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* term = bin1 * bin2 * pow */
        seq_t term = bin1;
        term = term * bin2;
        term = term * pow;
        sum += term;

        /* Prepare for next iteration, except after the last term */
        if (k + 1 == n) break;

        /* Update power: (n-3)^(k+1) */
        pow = pow * base;

        /* Update bin1: C(n+k, k+1) = C(n+k-1, k) * (n+k)/(k+1) */
        seq_t tmp1 = bin1 * ((seq_t)n + k);
        bin1 = tmp1 / ((seq_t)k + 1);

        /* Update bin2: C(3n, n-(k+1)-1) = C(3n, m-1) where m = n-k-1 */
        idx_t m = n - k - 1;          /* current m = n-k-1   */
        seq_t m_seq = (seq_t)m;
        seq_t denom = three_n - m_seq + 1;   /* = 2n + k + 2 */
        seq_t tmp2 = bin2 * m_seq;
        bin2 = tmp2 / denom;
    }

    return sum / (seq_t)n;
}
