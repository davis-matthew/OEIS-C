/*
 * A281970  Number of sets of exactly n positive integers <= n+7 having a
 *          square element sum.
 * https://oeis.org/A281970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of sets of exactly n positive integers <= n+7 having a
 *            square element sum
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 172, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed as the number of 7‑element subsets of {1,…,n+7} whose sum equals
   (n+7)(n+8)/2 minus a perfect square.  The counts of 7‑subsets by their sum are
   obtained from the Gaussian binomial coefficient [n+7 choose 7]_q. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The integer types idx_t and seq_t are supplied by the build. */

seq_t a(idx_t n)
{
    /* N = n + 7 : the size of the universe */
    const idx_t N = n + (idx_t)7;

    /* Preallocate polynomial coefficient arrays for k = 0..7.
       The maximum degree for fixed k is k*(N-k). */
    seq_t *coeff[8];
    for (int k = 0; k <= 7; ++k) {
        idx_t sz = 1;
        if (k > 0) {
            sz = k * (N - k) + 1;   /* +1 for the 0-index */
        }
        coeff[k] = (seq_t*)calloc((size_t)sz, sizeof(seq_t));
        /* calloc initializes memory to zero. */
    }
    /* Initialise the base case: [N choose 0] = 1. */
    coeff[0][0] = (seq_t)1;

    /* Build the Gaussian binomial [N choose k]_q for k = 0..7 incrementally. */
    for (idx_t cur = 1; cur <= N; ++cur) {
        int maxk = (int)(cur < 7 ? cur : 7);          /* k cannot exceed cur or 7 */
        for (int k = maxk; k >= 1; --k) {
            idx_t shift = cur - (idx_t)k;                 /* power of q to add */
            /* old_deg is the highest index used in coeff[k-1] before this step */
            idx_t old_deg = (k - 1) * ( (cur - 1) - (k - 1) );
            for (idx_t t = 0; t <= old_deg; ++t) {
                coeff[k][t + shift] += coeff[k - 1][t];
            }
        }
    }

    /* Total sum of the whole set {1..N} */
    const seq_t total_sum = (seq_t)N * (seq_t)(N + 1) / (seq_t)2;

    /* Minimum and maximum possible sums of 7 distinct numbers from {1..N} */
    const idx_t sum_min = 28;                          /* 1+2+...+7 */
    const idx_t sum_max = 7 * N - 21;                   /* N+ (N-1)+...+(N-6) */

    seq_t result = (seq_t)0;

    /* Enumerate all squares s^2 <= total_sum */
    for (idx_t s = 0;; ++s) {
        const seq_t sq = (seq_t)s * (seq_t)s;
        if (sq > total_sum) {
            break;
        }
        const seq_t excluded = total_sum - sq;
        if (excluded < sum_min || excluded > sum_max) {
            continue;
        }
        const idx_t t = (idx_t)(excluded - (seq_t)sum_min);
        result += coeff[7][t];
    }

    /* Free memory */
    for (int k = 0; k <= 7; ++k) {
        free(coeff[k]);
    }

    return result;
}
