/*
 * A359895  Number of odd-length integer partitions of n whose parts have the
 *          same mean as median.
 * https://oeis.org/A359895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd‑length integer partitions of n whose parts
 *            have mean = median. * For each odd divisor L of n with mean m =
 *            n / L the partitions are counted by * sum_{s=0}^{t*(m-1)}
 *            p_def(s, ≤t, parts≤m-1) * p_ex(s, ≤t) * where t = (L-1)/2. The
 *            function implements this counting using DP
 * Range:     u8 n <= 34, u16 n <= 74, u32 n <= 199, u64 n <= 584,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t total = 0;

    /* enumerate all divisors m of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* compute contributions for divisor d */
        {
            idx_t m = d;
            idx_t x = n / m;                /* L = x */
            if ((x & 1) == 1) {             /* L must be odd */
                idx_t t = (x - 1) / 2;      /* number of parts on each side of median */
                if (t == 0) {               /* L = 1 -> single part partition */
                    total += 1;
                } else {
                    idx_t M = m - 1;         /* largest deficit size */
                    if (M == 0) {            /* m = 1 -> only all‑ones partition */
                        total += 1;
                    } else {
                        idx_t maxS = t * M;
                        size_t cols = maxS + 1;

                        /* excess DP: partitions of s into at most t parts (unrestricted parts) */
                        seq_t *excess = (seq_t*)calloc(cols, sizeof(seq_t));
                        excess[0] = 1;
                        for (idx_t p = 1; p <= t; ++p) {
                            for (idx_t s = p; s <= maxS; ++s) {
                                excess[s] += excess[s - p];
                            }
                        }

                        /* deficits DP: partitions of s into at most t parts with part ≤ M */
                        size_t dp_rows = t + 1;
                        size_t dp_cols = cols;
                        size_t dp_size = dp_rows * dp_cols;
                        seq_t *dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
                        dp[0] = 1;   /* dp[0][0] = 1 */

                        for (idx_t dval = 1; dval <= M; ++dval) {
                            for (idx_t s = dval; s <= maxS; ++s) {
                                size_t s_idx = s;
                                for (idx_t u = 1; u <= t; ++u) {
                                    size_t cur = u * dp_cols + s_idx;
                                    size_t prev = (u - 1) * dp_cols + (s_idx - dval);
                                    dp[cur] += dp[prev];
                                }
                            }
                        }

                        /* deficits array: sum over u ≤ t */
                        seq_t *deficits = (seq_t*)calloc(cols, sizeof(seq_t));
                        for (idx_t u = 0; u <= t; ++u) {
                            size_t base = u * dp_cols;
                            for (idx_t s = 0; s <= maxS; ++s) {
                                deficits[s] += dp[base + s];
                            }
                        }

                        /* dot product of deficits and excess arrays */
                        seq_t sum_term = 0;
                        for (idx_t s = 0; s <= maxS; ++s) {
                            sum_term += deficits[s] * excess[s];
                        }
                        total += sum_term;

                        free(excess);
                        free(dp);
                        free(deficits);
                    }
                }
            }
        }

        /* compute contributions for the paired divisor n/d if distinct */
        if (d * d != n) {
            idx_t m = n / d;
            idx_t x = n / m;                /* x = d */
            if ((x & 1) == 1) {
                idx_t t = (x - 1) / 2;
                if (t == 0) {
                    total += 1;
                } else {
                    idx_t M = m - 1;
                    if (M == 0) {
                        total += 1;
                    } else {
                        idx_t maxS = t * M;
                        size_t cols = maxS + 1;

                        seq_t *excess = (seq_t*)calloc(cols, sizeof(seq_t));
                        excess[0] = 1;
                        for (idx_t p = 1; p <= t; ++p) {
                            for (idx_t s = p; s <= maxS; ++s) {
                                excess[s] += excess[s - p];
                            }
                        }

                        size_t dp_rows = t + 1;
                        size_t dp_cols = cols;
                        size_t dp_size = dp_rows * dp_cols;
                        seq_t *dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
                        dp[0] = 1;

                        for (idx_t dval = 1; dval <= M; ++dval) {
                            for (idx_t s = dval; s <= maxS; ++s) {
                                for (idx_t u = 1; u <= t; ++u) {
                                    size_t cur = u * dp_cols + s;
                                    size_t prev = (u - 1) * dp_cols + (s - dval);
                                    dp[cur] += dp[prev];
                                }
                            }
                        }

                        seq_t *deficits = (seq_t*)calloc(cols, sizeof(seq_t));
                        for (idx_t u = 0; u <= t; ++u) {
                            size_t base = u * dp_cols;
                            for (idx_t s = 0; s <= maxS; ++s) {
                                deficits[s] += dp[base + s];
                            }
                        }

                        seq_t sum_term = 0;
                        for (idx_t s = 0; s <= maxS; ++s) {
                            sum_term += deficits[s] * excess[s];
                        }
                        total += sum_term;

                        free(excess);
                        free(dp);
                        free(deficits);
                    }
                }
            }
        }
    }
    return total;
}
