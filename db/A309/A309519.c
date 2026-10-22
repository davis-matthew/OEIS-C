/*
 * A309519  Sum of the even parts in the partitions of n into 4 parts.
 * https://oeis.org/A309519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the even parts in the partitions of n into 4
 *            parts. * The loops below follow the OEIS summation: * a(n) =
 *            Σ_{k=1..⌊n/4⌋} Σ_{j=k..⌊(n-k)/3⌋} * Σ_{i=j..⌊(n-j-k)/2⌋}
 *            (i·((i-1) mod 2)+j·((j-1) mod 2)+ * k·((k-1) mod
 *            2)+(n-i-j-k)·((n-i-j-k-1) mod 2)) * * The implementation
 *            reformulates t...
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   even_contrib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t etc. (idx_t, seq_t are supplied by the build) */
#include "../utils/even_contrib.h"

/* Main sequence function */
seq_t a(idx_t n) {
    /* For n < 4 there are no partitions into 4 positive parts */
    if (n < 4)
        return 0;

    seq_t total = 0;

    /* k is the smallest part */
    for (idx_t k = 1; k <= n / 4; ++k) {
        seq_t k_even = even_contrib(k);

        /* j is the second part (≥ k) */
        idx_t j_max = (n - k) / 3;
        for (idx_t j = k; j <= j_max; ++j) {
            seq_t j_even = even_contrib(j);

            /* i is the third part, ranges from j to ⌊(n-k-j)/2⌋ */
            idx_t t = (n - k - j) / 2;            /* upper bound for i */
            if (t < j)
                continue;                        /* no valid i for this j */

            idx_t count_i = t - j + 1;           /* how many i's */
            seq_t count_seq = (seq_t)count_i;

            /* Contribution of the fixed k and j across all i's */
            seq_t partial_k_j = (k_even + j_even) * count_seq;

            /* Sum of even i-values in the range [j, t] */
            idx_t first_even = j;
            if (first_even & 1)  first_even++;   /* make even */
            idx_t last_even = t;
            if (last_even & 1)   last_even--;    /* make even */
            seq_t sum_even_i = 0;
            if (first_even <= last_even) {
                idx_t cnt_even = (last_even - first_even) / 2 + 1;
                sum_even_i = (seq_t)cnt_even *
                             ((seq_t)first_even + (seq_t)last_even) / 2;
            }

            /* Even parts contributed by the fourth part (d = n-i-j-k) */
            idx_t parity_d = (n - k - j) & 1;     /* 0 if d is even */
            idx_t first_par = j;
            if ((first_par & 1) != parity_d) first_par++;   /* first i with correct parity */
            idx_t last_par = t;
            if ((last_par & 1) != parity_d) last_par--;     /* last i with correct parity */
            seq_t sum_even_d = 0;
            if (first_par <= last_par) {
                idx_t cnt_par = (last_par - first_par) / 2 + 1;
                seq_t sum_par =
                    (seq_t)cnt_par *
                    ((seq_t)first_par + (seq_t)last_par) / 2;
                sum_even_d = (seq_t)cnt_par *
                              ((seq_t)(n - k - j)) -
                              sum_par;
            }

            /* Add all contributions for this (k, j) pair */
            total += partial_k_j + sum_even_i + sum_even_d;
        }
    }

    return total;
}
