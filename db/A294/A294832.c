/*
 * A294832  Denominators of the partial sums of the reciprocals of the numbers
 *          (k + 1)*(5*k + 4) = 2*A005476(k+1), for k >= 0.
 * https://oeis.org/A294832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of Σ_{k=0..n} 1/((k+1)*(5*k+4))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Multiply two seq_t values using 128‑bit arithmetic if available,
 * otherwise fall back to the built‑in multiply.  This keeps the
 * intermediate product from overflowing the target type for all
 * arguments that fit into seq_t. */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
#ifdef __SIZEOF_INT128__
    return (seq_t)((unsigned __int128)a * b);
#else
    return a * b;
#endif
}

seq_t a(idx_t n)
{
    seq_t num = 0;   /* numerator of the current sum in lowest terms */
    seq_t den = 1;   /* denominator of the current sum in lowest terms */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t k1   = (seq_t)(i + 1);           /* k+1 */
        seq_t k5   = (seq_t)(5 * i + 4);       /* 5k+4 */
        seq_t term = mul_seq(k1, k5);          /* (k+1)*(5k+4) */

        /* Add 1/term to the fraction num/den  */
        seq_t g1       = gcd(den, term);
        seq_t den_div  = den / g1;             /* den/g1   */
        seq_t term_div = term / g1;            /* term/g1  */

        /* LCM(den, term) = den_div * term */
        seq_t new_den = mul_seq(den_div, term);

        /* num/den + 1/term -> new numerator */
        seq_t tmp      = mul_seq(num, term_div);   /* num * term_div */
        seq_t new_num  = tmp + den_div;            /* + den/g1 */

        /* Reduce the fraction to lowest terms */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
