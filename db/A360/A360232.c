/*
 * A360232  G.f. Sum_{n>=0} a(n)*x^n = Sum_{n>=0} (1 + n*x + x^2)^n * x^n.
 * https://oeis.org/A360232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=ceil(n/3)..n}
 *                 (Sum_{j=ceil((n-k)/2)..min(k,n-k)}
 *                     binomial(k,j) * binomial(j,n-k-j) * k^(2*j-(n-k))
 *
 * Implementation uses an iterative recurrence for binomial coefficients
 * and powers, avoiding expensive factorial or binomial function calls.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = 0;

    /* k ranges from ceil(n/3) to n */
    idx_t k_min = (n + 2) / 3;
    for (idx_t k = k_min; k <= n; ++k)
    {
        idx_t m = n - k;                  /* m = n - k */
        idx_t j_min = (m + 1) / 2;        /* ceil(m/2) */
        idx_t j_max = (m < k) ? m : k;    /* min(k,m) */
        if (j_min > j_max) continue;

        /* initial binomial(k, j_min) */
        seq_t bin_k_j = 1;
        for (idx_t jj = 0; jj < j_min; ++jj)
            bin_k_j = bin_k_j * (seq_t)(k - jj) / (seq_t)(jj + 1);

        /* initial binomial(j_min, m - j_min) */
        idx_t l = m - j_min;              /* l = m - j_min = n - k - j_min */
        seq_t bin_j_l = 1;
        for (idx_t jj = 0; jj < l; ++jj)
            bin_j_l = bin_j_l * (seq_t)(j_min - jj) / (seq_t)(jj + 1);

        /* exponent of k for the first term: 2*j_min - m (0 or 1) */
        seq_t pow_k_curr = (m & 1) ? (seq_t)k : (seq_t)1;

        for (idx_t j = j_min; j <= j_max; ++j)
        {
            /* term = binomial(k,j) * binomial(j,m-j) * k^(2*j - m) */
            seq_t term = bin_k_j * bin_j_l;
            term *= pow_k_curr;
            result += term;

            if (j == j_max) break;        /* last j, no further update */

            /* Prepare for next j */
            idx_t l_cur = m - j;          /* l_cur = m-j for current j */

            /* bin(j, l-1) from bin(j,l) */
            seq_t bin_j_l_minus1 = bin_j_l * (seq_t)l_cur / (seq_t)(j - l_cur + 1);

            /* bin(j+1, l-1) from bin(j, l-1) */
            seq_t bin_next = bin_j_l_minus1 *
                             (seq_t)(j + 1) /
                             (seq_t)(j - l_cur + 2);
            bin_j_l = bin_next;

            /* bin(k, j+1) from bin(k, j) */
            bin_k_j = bin_k_j * (seq_t)(k - j) / (seq_t)(j + 1);

            /* increase exponent by 2: multiply by k^2 */
            pow_k_curr *= (seq_t)k * (seq_t)k;
        }
    }
    return result;
}
