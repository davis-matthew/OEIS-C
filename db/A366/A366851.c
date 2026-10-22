/*
 * A366851  Irregular triangle read by rows where T(n,k) is the number of
 *          integer partitions of n such that the sum of primes indexed by all
 *          parts greater than one is k.
 * https://oeis.org/A366851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) counts integer partitions of n whose sum of primes
 *            indexed by parts > 1 equals k. a(n) is the flattening of the
 *            triangle row by row
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the n-th element of the flattened
   triangle.  It is implemented by precomputing primes up to
   the 200th prime, then building all rows up to and beyond
   the index needed (the 200th row is more than enough for
   the first 10 000 terms).  The DP reuses the classic
   unbounded knapsack approach: for each part size p we
   update all sums n and weights k. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ROW 200               /* enough for n up to 10000 */
#define MAX_PRIME 3000            /* enough to find the 200th prime */
#define MAX_K 2005                /* maximum possible prime(idx)+1 */

static bool initialized = false;
static idx_t prime_idx[MAX_ROW + 1];
static idx_t start_idx[MAX_ROW + 1];
static seq_t dp[MAX_ROW + 1][MAX_K];

static void precompute(void)
{
    bool comp[MAX_PRIME + 1] = {0};
    idx_t primes_list[MAX_ROW + 1];
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= MAX_PRIME && prime_cnt <= MAX_ROW; ++i) {
        if (!comp[i]) {
            primes_list[prime_cnt++] = i;
            if (i * i <= MAX_PRIME)
                for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                    comp[j] = true;
        }
    }

    prime_idx[0] = 0;
    prime_idx[1] = 0;
    for (idx_t i = 2; i <= MAX_ROW; ++i)
        prime_idx[i] = primes_list[i - 1];   /* nth prime, 1‑based */

    /* prefix starting indices of each row in the flattened array */
    start_idx[0] = 0;
    for (idx_t r = 0; r < MAX_ROW; ++r) {
        idx_t len = (r < 2) ? 1 : prime_idx[r] + 1;
        start_idx[r + 1] = start_idx[r] + len;
    }

    /* initialize DP table */
    for (idx_t r = 0; r <= MAX_ROW; ++r)
        for (idx_t k = 0; k < MAX_K; ++k)
            dp[r][k] = 0;
    dp[0][0] = 1;

    /* unbounded knapsack DP for partitions with weight = prime(part) */
    for (idx_t p = 1; p <= MAX_ROW; ++p) {
        idx_t weight = (p == 1) ? 0 : prime_idx[p];
        for (idx_t sum = p; sum <= MAX_ROW; ++sum) {
            idx_t max_k = (sum < 2) ? 0 : prime_idx[sum];
            idx_t max_k_prev = (sum < 2) ? 0 : prime_idx[sum - p];
            for (idx_t k = weight; k <= max_k; ++k) {
                if (k - weight <= max_k_prev)
                    dp[sum][k] += dp[sum - p][k - weight];
            }
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        precompute();

    /* locate the row that contains the index n */
    idx_t r = 0;
    while (r < MAX_ROW && start_idx[r + 1] <= n)
        ++r;
    if (r > MAX_ROW)
        return 0;  /* out of bounds, should not happen for n <= 10000 */

    idx_t k = n - start_idx[r];
    if (k >= MAX_K)
        return 0;   /* safety */

    return dp[r][k];
}
