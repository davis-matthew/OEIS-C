/*
 * A258360  Sum over all partitions lambda of n into 5 distinct parts of
 *          Product_{i:lambda} prime(i).
 * https://oeis.org/A258360
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 51, u64 n <= 410, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A258360
 *
 *  a(n) = Sum over all partitions λ of n into 5 distinct positive parts
 *         of  ∏_{i∈λ} prime(i)
 *
 *  The function below computes a(n) for any n ≥ 15 using a dynamic
 *  programming approach that builds the required sums for each number
 *  of parts.  The implementation is fully compliant with C11 and
 *  C++17 and uses only the operations allowed for seq_t and idx_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  One-time initialization of the first N primes.  The array
    `prime_list` holds primes in ascending order;
    prime_list[i] == prime(i+1).  This function builds enough
    primes for the requested n.  We use a generous fixed limit
    that is sufficient for n up to at least 10000. */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(idx_t n)
{
    if (prime_list != NULL)
        return;  /* already initialized */

    /* The 10000th prime is 104729; 200000 is a safe upper bound. */
    const idx_t LIMIT = 200000;
    bool *is_prime = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!is_prime)
        abort();
    for (idx_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = true;
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= LIMIT; j += p)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++cnt;
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list)
        abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[pos++] = (uint32_t)i;
    prime_count = cnt;
    free(is_prime);
}

/*  The main sequence function.  It uses a 2‑dimensional DP
    table dp[t][s] that stores, for each t (0…5) and sum s,
    the sum of products of primes of all partitions of s into
    t distinct parts. */
seq_t a(idx_t n)
{
    if (n < 15)
        return 0;                       /* values below offset are irrelevant */

    init_primes(n);                    /* initialise primes once */

    /* Allocate DP table: 6 rows, n+1 columns */
    seq_t **dp = (seq_t **)malloc(6 * sizeof(seq_t *));
    if (!dp)
        abort();
    for (int t = 0; t <= 5; ++t) {
        dp[t] = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (!dp[t])
            abort();
    }

    dp[0][0] = 1;          /* empty partition product = 1 */

    /* Build DP: iterate over all possible part sizes (1 … n) */
    for (idx_t part = 1; part <= n; ++part) {
        seq_t pval = (seq_t)prime_list[part - 1];
        /* Update from higher t to lower to avoid reuse of the same part */
        for (int t = 5; t >= 1; --t) {
            for (idx_t s = n; s >= part; --s) {
                seq_t add = dp[t - 1][s - part];
                if (add != 0)
                    dp[t][s] += add * pval;
            }
        }
    }

    seq_t result = dp[5][n];

    /* Free memory */
    for (int t = 0; t <= 5; ++t)
        free(dp[t]);
    free(dp);

    return result;
}
