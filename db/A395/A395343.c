/*
 * A395343  a(n) = (2*n)! * [x^(2*n)] (cosh(x/2) / cos(x/2))^2.
 * https://oeis.org/A395343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * [x^(2*n)] (cosh(x/2) / cos(x/2))^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: B_m = (m! * c_m) is computed by */
/*    B_m + Σ_{k even, 2≤k≤m} (-1)^{k/2} binom(m,k) B_{m-k} = N'_m   */
/* where N'_0 = 2, N'_m = 1 for even m>0, N'_m = 0 for odd m. */
/* Finally a(n) = B_{2n}. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Ensure that seq_t and idx_t are defined by the build environment */
seq_t a(idx_t n) {
    /* Handle offset 0 directly */
    if (n == 0) return (seq_t)1;

    static seq_t *B = NULL;        /* array of B_m values, indexed by m */
    static idx_t cur_m = 0;        /* highest m currently computed */

    idx_t need = 2 * n;            /* we need B up to index need */

    /* Allocate or grow the array if necessary */
    if (B == NULL) {
        B = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        if (B == NULL) return (seq_t)0;   /* allocation failed */
        B[0] = (seq_t)1;                  /* B_0 = 1 */
        cur_m = 0;
    } else if (need > cur_m) {
        seq_t *tmp = (seq_t *)realloc(B, (need + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        B = tmp;
    }

    /* Compute B for m = cur_m+1 .. need */
    for (idx_t m = cur_m + 1; m <= need; ++m) {
        /* Determine N'_m */
        seq_t N;
        if ((m & 1) == 0)               /* even */
            N = (m == 0) ? (seq_t)2 : (seq_t)1;
        else
            N = (seq_t)0;

        seq_t sum_pos = 0;      /* sum of terms with positive sign */
        seq_t sum_neg = 0;      /* sum of terms with negative sign */

        /* bin = binomial(m,0) = 1 initially */
        seq_t bin = (seq_t)1;

        for (idx_t k = 2; k <= m; k += 2) {
            /* Update bin to binomial(m,k) from binomial(m,k-2) */
            seq_t temp = bin;
            temp = temp * (seq_t)(m - k + 2);
            temp = temp * (seq_t)(m - k + 1);
            temp = temp / ( (seq_t)k * (k - 1) );
            bin = temp;

            /* term = bin * B[m-k] */
            seq_t term = bin * B[m - k];

            /* sign: (-1)^{k/2} -> positive if (k/2) even, negative otherwise */
            if ( (((k >> 1) & 1) == 0) )
                sum_pos += term;
            else
                sum_neg += term;
        }

        /* Compute numerator = N - (sum_pos - sum_neg) or N + (sum_neg - sum_pos) */
        seq_t diff, numerator;
        if (sum_pos >= sum_neg) {
            diff = sum_pos - sum_neg;
            numerator = N - diff;
        } else {
            diff = sum_neg - sum_pos;
            numerator = N + diff;
        }

        /* B_m = numerator / 2 */
        B[m] = numerator / (seq_t)2;
    }

    cur_m = need;
    return B[need];
}
