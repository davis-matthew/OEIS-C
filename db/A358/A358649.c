/*
 * A358649  Number of convergent n X n matrices over GF(2).
 * https://oeis.org/A358649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^{(n-k)(n-1)} * G(n,k,2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where G(n,k,2) is the Gaussian binomial coefficient over GF(2). */
/*  Using the recurrence G(n,k)=G(n-1,k)+2^{n-k}*G(n-1,k-1), the summand
 *  can be generated iteratively with only shifts, small arithmetic and
 *  division by the odd numbers (2^{k+1}-1).  The algorithm runs in O(n)
 *  time per call. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;

    /* pre‑compute powers of two up to 2^n (they are small, at most 2^10000) */
    size_t len = (size_t)(n + 1);
    seq_t *pow2 = (seq_t *)malloc(sizeof(seq_t) * len);
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] << 1;              /* 2^i */

    /* initial term for k = 0  -> G(n,0)=1  */
    seq_t term = ((seq_t)1) << (n * (n - 1));
    seq_t sum  = term;

    for (idx_t k = 0; k < n; ++k) {
        /* numerator = 2^{n-k} - 1 */
        seq_t num = pow2[n - k] - (seq_t)1;
        /* denominator = 2^{k+1} - 1 */
        seq_t den = pow2[k + 1] - (seq_t)1;

        /* term * (2^{n-k} - 1) == (term << (n-k)) - term */
        seq_t shifted = term << (n - k);
        seq_t tmp     = shifted - term;          /* exact multiplication */

        /* divide by the odd denominator */
        tmp = tmp / den;

        /* shift right by (n-1) to apply the factor 2^{-(n-1)} */
        tmp = tmp >> (n - 1);

        term = tmp;
        sum += term;
    }

    free(pow2);
    return sum;
}
