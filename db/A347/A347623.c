/*
 * A347623  Number of partitions of n into at most 4 prime parts (counting 1
 *          as a prime).
 * https://oeis.org/A347623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into at most 4 prime parts (1 counted
 *            as a prime)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case directly */
    if (n == 0U)
        return (seq_t)1;

    /* --------------------------------------------------------------------
     *  Prime generation (classic sieve of Eratosthenes) up to n.
     *  The result is stored in a dynamically sized array of uint64_t.
     * -------------------------------------------------------------------- */
    bool *is_prime = (bool*)malloc((n + 1) * sizeof(bool));
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                is_prime[m] = false;
        }
    }

    /* Count primes to allocate the array */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++prime_cnt;

    uint64_t *primes = (uint64_t*)malloc((prime_cnt + 1) * sizeof(uint64_t));
    size_t idx = 0;
    /* 1 is treated as a “prime” in this problem */
    primes[idx++] = 1;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            primes[idx++] = (uint64_t)i;   /* idx should equal prime_cnt + 1 now */

    free(is_prime);

    /* --------------------------------------------------------------------
     *  Dynamic programming table dp[parts][sum]
     *  dp is a flattened 2‑D array of size 5 × (n+1).
     *  We must allocate it correctly for C++ (new[]) and C (malloc).
     * -------------------------------------------------------------------- */
    size_t rows   = 5;                 /* 0 .. 4 parts */
    size_t columns = (size_t)n + 1;    /* sums 0 .. n */
    size_t len    = rows * columns;    /* total entries */

#if defined(__cplusplus)
    seq_t *dp = new seq_t[len]();     /* value‑initialised to zero */
#else
    seq_t *dp = (seq_t*)malloc(len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i)
        dp[i] = (seq_t)0;
#endif

    /* Base case: one way to reach sum 0 with 0 parts */
    dp[0 * columns + 0] = (seq_t)1;

    /* --------------------------------------------------------------------
     *  Iterate over the “prime” numbers (including 1) and update dp
     * -------------------------------------------------------------------- */
    for (size_t pi = 0; pi < idx; ++pi) {
        uint64_t p = primes[pi];
        for (size_t j = 1; j <= 4; ++j) {
            size_t row = j * columns;
            size_t prev_row = (j - 1) * columns;
            for (idx_t s = p; s <= n; ++s) {
                /* dp[row][s] += dp[prev_row][s - p] */
                dp[row + (size_t)s] += dp[prev_row + (size_t)(s - p)];
            }
        }
    }

    /* --------------------------------------------------------------------
     *  Sum over all allowed part counts (1..4) for the target sum n.
     * -------------------------------------------------------------------- */
    seq_t result = (seq_t)0;
    for (size_t j = 1; j <= 4; ++j)
        result += dp[j * columns + (size_t)n];

    /* --------------------------------------------------------------------
     *  Clean up and return.
     * -------------------------------------------------------------------- */
    free(primes);
#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
