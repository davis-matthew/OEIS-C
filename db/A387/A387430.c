/*
 * A387430  a(n) = Sum_{k=0..n} (n-i)^k * (n+i)^(n-k) * binomial(n,k)^2, where
 *          i is the imaginary unit.
 * https://oeis.org/A387430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n^2+1)^k * (2*n)^(n-2k) *
 *            binomial(n,2k) * binomial(2k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for seq_t with an unsigned integer exponent */
static inline seq_t seq_pow_uint(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t cur = base;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= cur;
        cur *= cur;
        e >>= 1;
    }
    return result;
}

/* a(n) implementation */
seq_t a(idx_t n) {
    /* special case n = 0 */
    if (n == 0) return 1;

    /* precompute constants */
    seq_t n_sq_plus1 = (seq_t)n * (seq_t)n + (seq_t)1;          /* n^2 + 1          */
    seq_t two_n       = (seq_t)(2 * n);                        /* 2 * n             */
    seq_t two_n_sq    = two_n * two_n;                        /* (2n)^2            */
    idx_t maxk        = n / 2;                                 /* floor(n/2)        */

    /* initial values */
    seq_t pow1 = 1;                                          /* (n^2+1)^0         */
    seq_t pow2 = seq_pow_uint(two_n, n);                     /* (2n)^n            */
    seq_t bin1 = 1;                                          /* binomial(n,0)     */
    seq_t bin2 = 1;                                          /* binomial(0,0)     */
    seq_t sum  = 0;

    idx_t k;
    for (k = 0; k <= maxk; ++k) {
        /* term = pow1 * pow2 * bin1 * bin2 */
        seq_t term = pow1 * pow2;
        term *= bin1;
        term *= bin2;
        sum += term;

        if (k == maxk) break; /* avoid updating when loop ends */

        /* Update pow1: multiply by (n^2+1) */
        pow1 *= n_sq_plus1;

        /* Update pow2: divide by (2n)^2 (exact division) */
        pow2 /= two_n_sq;

        /* Update bin1 for next even m = 2*(k+1) */
        /* current m = 2*k */
        idx_t m = 2 * k;
        /* new bin1 = bin1 * (n - m) * (n - m - 1) / ((m + 1) * (m + 2)) */
        seq_t num = bin1 * (seq_t)(n - m);
        num *= (seq_t)(n - m - 1);
        seq_t denom = (seq_t)(m + 1) * (seq_t)(m + 2);
        bin1 = num / denom;   /* exact division */

        /* Update bin2 for next k: B(k+1) = B(k) * (4k+2) / (k+1) */
        bin2 = (bin2 * (seq_t)(4 * k + 2)) / (seq_t)(k + 1);
    }

    return sum;
}
