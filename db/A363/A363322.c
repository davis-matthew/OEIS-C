/*
 * A363322  Total number of parts coprime to n in the partitions of n into 4
 *          parts.
 * https://oeis.org/A363322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..⌊n/4⌋} Sum_{j=k..⌊(n-k)/3⌋}
 *            Sum_{i=j..⌊(n-j-k)/2⌋} (c(i) + c(j) + c(k) + c(n-i-j-k)) where
 *            c(x) = [gcd(n,x) = 1]
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* No partitions for n < 4 */
    if (n < 4) return 0;

    /* Convert to size_t for array handling */
    const size_t N = (size_t)n;

    /* Allocate arrays for coprime flags and prefix sums */
    seq_t *carr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *pref = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!carr || !pref) {
        /* Allocation failed – return 0 to avoid crash */
        return 0;
    }

    /* Compute coprime flag and prefix sums */
    carr[0] = 0;
    pref[0] = 0;
    for (size_t x = 1; x <= N; ++x) {
        seq_t g = gcd((seq_t)n, (seq_t)x);
        carr[x] = (g == 1) ? 1 : 0;
        pref[x] = pref[x - 1] + carr[x];
    }

    seq_t total = 0;
    idx_t max_k = n / 4;

    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t max_j = (n - k) / 3;
        for (idx_t j = k; j <= max_j; ++j) {
            idx_t L = n - k - j;          /* Remaining sum for two parts */
            if (L < 2 * j) continue;     /* No valid i if 2*j > L */

            idx_t i_min = j;
            idx_t i_max = L / 2;
            idx_t cnt   = i_max - i_min + 1;

            /* Number of copies contributed by k and j */
            seq_t part1 = (seq_t)cnt *
                          ((seq_t)carr[(size_t)k] + (seq_t)carr[(size_t)j]);

            /* Sum of c(i) over i in [i_min, i_max] */
            seq_t sum_c_i = pref[(size_t)i_max] - pref[(size_t)(i_min - 1)];

            /* Corresponding l values: l = L - i */
            idx_t l_min = L - i_max;
            idx_t l_max = L - i_min;
            seq_t sum_c_l = pref[(size_t)l_max] - pref[(size_t)(l_min - 1)];

            total += part1 + sum_c_i + sum_c_l;
        }
    }

    free(carr);
    free(pref);
    return total;
}
