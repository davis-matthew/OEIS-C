/*
 * A253194  Number of ways to place non-intersecting diagonals in a convex
 *          (n+2)-gon so as to create no pentagons.
 * https://oeis.org/A253194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/(n+1)) *
 *   [ Sum_{i=0..⌊n/3⌋} Sum_{k=i+1..n-2i} (-1)^i *
 *        binomial(n+k, k) * binomial(k, i) * binomial(n-3i-1, k-i-1)
 *     + (if n≡0 (mod 3)) (-1)^{n/3} * binomial(4n/3, n/3) ]
 *
 * The implementation evaluates the double sum using an incremental
 * recurrence to avoid re-computing binomial coefficients from
 * scratch for each k.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* Provides binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* sequence indices are 1-based; values for n < 1 are undefined */
    if (n == 0) return 0;

    seq_t pos_sum = 0;   /* sum of terms with (+) sign          */
    seq_t neg_sum = 0;   /* sum of terms with (-) sign          */

    /* General double sum */
    idx_t i_max = n / 3;                     /* floor(n/3) */
    for (idx_t i = 0; i <= i_max; ++i) {
        idx_t k_min = i + 1;
        idx_t k_max = n - 2 * i;
        if (k_min > k_max) continue;         /* no valid k for this i */

        /* Initial term for k = k_min */
        seq_t term = binomial(n + k_min, k_min);
        term = term * binomial(k_min, i);
        term = term * binomial(n - 3 * i - 1, k_min - i - 1);

        /* sign for this value of i */
        int sign_pos = (i & 1) == 0;          /* +1 if even i, else -1 */

        if (sign_pos) pos_sum += term; else neg_sum += term;

        /* Iterate over k from k_min+1 to k_max */
        for (idx_t k = k_min; k < k_max; ++k) {
            /* Update term to the next k using the recurrence:
             *   term_{k+1} = term_k * (n+k+1)*(n-2i-k) / ((k+1-i)*(k-i))
             */
            seq_t num = (seq_t)(n + k + 1) * (seq_t)(n - 2 * i - k);
            seq_t den = (seq_t)(k + 1 - i) * (seq_t)(k - i);

            term = term * num;
            term = term / den;

            if (sign_pos) pos_sum += term; else neg_sum += term;
        }
    }

    /* Special outer term when n ≡ 0 (mod 3) */
    if ((n % 3) == 0) {
        idx_t div3 = n / 3;
        seq_t special = binomial(4 * div3, div3);
        int sign_pos = (div3 & 1) == 0;
        if (sign_pos) pos_sum += special; else neg_sum += special;
    }

    /* Final result: (pos_sum - neg_sum) / (n+1) */
    seq_t total = pos_sum - neg_sum;
    return total / ((seq_t)(n + 1));
}
