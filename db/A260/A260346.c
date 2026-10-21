/*
 * A260346  Expansion of x^2*((1 - 12*x + 50*x^2 - 76*x^3 + 42*x^4 - 48*x^5 +
 *          32*x^6)/(1 - 4*x)^4 + 4*x^2/(1 - 4*x)^(5/2)).
 * https://oeis.org/A260346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A260346: coefficient extraction from
 * x^2 * ((1-12x+50x^2-76x^3+42x^4-48x^5+32x^6)/(1-4x)^4 + 4x^2/(1-4x)^(5/2))
 * 
 * The formula is evaluated directly.  For n < 2 the sequence is 0.
 * For n ≥ 2 the value is the sum of two parts:
 *
 *   part1(n) = Σ_{m=0..6} N_m · C(k+3,3) · 4^k   where k = n-2-m  (if k ≥ 0)
 *   part2(n) = 4 · b_{n-4}    (for n ≥ 4),
 *       with   b_0 = 1
 *       and   b_k = b_{k-1} · (4k + 6) / k  for k ≥ 1.
 *
 * N = {1, -12, 50, -76, 42, -48, 32}.
 * All arithmetic is performed with the seq_t type.  Signed coefficients
 * are handled by keeping positive and negative contributions separate.
 */

#include <stddef.h>   /* for size_t */

static const int N[7] = {1, -12, 50, -76, 42, -48, 32};

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;    /* offset 0: a(0)=0, a(1)=0 */

    /* ----- part 1 ----- */
    idx_t kmax = n - 2;                /* k = n-2-m, maximal when m=0 */
    seq_t pow4_all = 1;
    for (idx_t i = 0; i < kmax; ++i)
        pow4_all *= 4;                 /* 4^(n-2) */

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    seq_t pow_curr = pow4_all;

    for (int m = 0; m < 7; ++m) {
        if ((idx_t)m > n - 2)           /* k < 0, no further terms */
            break;

        idx_t k = n - 2 - m;           /* current power exponent */

        /* C(k+3,3) = (k+3)(k+2)(k+1)/6 */
        seq_t binom = ((seq_t)(k + 3) * (k + 2) * (k + 1)) / 6;

        seq_t term = binom * pow_curr;
        int   coeff = N[m];
        if (coeff >= 0)
            pos_sum += term * (seq_t)coeff;
        else
            neg_sum += term * (seq_t)(-coeff);

        /* prepare next term: divide by 4 (k decreases by 1) */
        if (pow_curr != 0)
            pow_curr /= 4;
    }

    seq_t part1 = pos_sum - neg_sum;

    /* ----- part 2 ----- */
    seq_t part2 = 0;
    if (n >= 4) {
        idx_t k = n - 4;                /* coefficient index */
        seq_t b = 1;
        for (idx_t i = 1; i <= k; ++i) {
            seq_t numer = 4 * i + 6;    /* 4i + 6 */
            b = (b * numer) / i;       /* exact division */
        }
        part2 = b * 4;
    }

    return part1 + part2;
}
