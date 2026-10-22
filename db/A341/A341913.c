/*
 * A341913  Number of partitions of n into 9 distinct and relatively prime
 *          parts.
 * https://oeis.org/A341913
 *
 * Offset:    45    (a(45) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * g(n/d) where g(m) is the number of
 *            partitions of m into 9 distinct parts
 * Range:     u8 n <= 62, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper arrays stored in static state */
static seq_t *dp = NULL;          /* dp[sum*10 + parts] */
static size_t dp_size = 0;        /* largest sum for which dp is valid */
static int *mu = NULL;            /* Möbius function values */
static idx_t mu_size = 0;         /* largest n for which mu is computed */

/* Allocate or resize the dp array for sums up to N (inclusive) */
static void update_dp(idx_t N)
{
    idx_t new_size = N + 1;              /* number of sums */
    size_t new_elem = new_size * 10;     /* 10 columns (0..9 parts) */

    /* Allocate new array and compute it from scratch */
    seq_t *new_dp = (seq_t *)malloc(new_elem * sizeof(seq_t));
    if (!new_dp) exit(1);
    for (idx_t i = 0; i < new_elem; ++i) new_dp[i] = 0;
    new_dp[0] = 1;                       /* dp[0*10+0] = 1 */

    /* Classic subset‑sum DP with distinct numbers (each number i used at most once) */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t s = N; s >= i; --s) {
            for (int k = 1; k <= 9; ++k) {
                new_dp[s * 10 + k] += new_dp[(s - i) * 10 + (k - 1)];
            }
        }
    }

    /* Free old array and replace */
    free(dp);
    dp = new_dp;
    dp_size = new_size;
}

/* Compute Möbius values up to N (inclusive) */
static void update_mu(idx_t N)
{
    int *new_mu = (int *)realloc(mu, (N + 1) * sizeof(int));
    if (!new_mu) exit(1);
    mu = new_mu;

    for (idx_t i = 0; i <= N; ++i) mu[i] = 1;
    mu[0] = 0;
    mu[1] = 1;

    for (idx_t p = 2; p <= N; ++p) {
        if (mu[p] == 1) {               /* prime */
            for (idx_t j = p; j <= N; j += p)
                mu[j] *= -1;
            idx_t sq = p * p;
            if (sq <= N) {
                for (idx_t j = sq; j <= N; j += sq)
                    mu[j] = 0;
            }
        }
    }
    mu_size = N;
}

seq_t a(idx_t n)
{
    if (n < 45) return 0;               /* undefined for n < 45 */

    /* Ensure dp and mu tables cover sums up to n */
    if (n + 1 > dp_size) update_dp(n);
    if (n + 1 > mu_size) update_mu(n);

    seq_t positive = 0, negative = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* First divisor d */
            int mu_d = mu[d];
            if (mu_d == 1) {
                positive += dp[(n / d) * 10 + 9];
            } else if (mu_d == -1) {
                negative += dp[(n / d) * 10 + 9];
            }

            /* Second divisor n/d if different */
            idx_t d2 = n / d;
            if (d2 != d) {
                int mu_d2 = mu[d2];
                if (mu_d2 == 1) {
                    positive += dp[(n / d2) * 10 + 9];
                } else if (mu_d2 == -1) {
                    negative += dp[(n / d2) * 10 + 9];
                }
            }
        }
    }

    return positive - negative;
}
