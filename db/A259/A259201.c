/*
 * A259201  Number of partitions of n into ten primes.
 * https://oeis.org/A259201
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 197, u32 n <= 1213, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A259201 – number of partitions of n into exactly ten primes.
 *  Formula (OEIS):  a(n) = [x^n y^10] ∏_{p prime} 1/(1 - x^p y)
 *  Algorithm: dynamic programming over primes, counting
 *  partitions with unlimited repetitions of each prime.
 *  The program uses a 2‑D DP table flattened into a 1‑D array:
 *  dp[k][s] = number of ways to write s as sum of k primes.
 *  Complexity: O(#primes(n) · 10 · n), fast enough for n ≤ 10 000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------- Helpers for prime generation ----------------------------------- */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime)
        return;                   /* allocation failure → empty list */

    for (size_t i = 2; i < sz; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    uint64_t *list = (uint64_t*)malloc(count * sizeof(uint64_t));
    if (!list) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            list[idx++] = (uint64_t)i;

    free(is_prime);
    *primes_out = list;
    *cnt_out = count;
}

/* ------- Sequence evaluation -------------------------------------------- */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static idx_t   max_n = 0;
    static seq_t * dp_arr = NULL;   /* flattened 2×(n+1) array, k=0..10 */
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;

    /* we need at least 'n'; if larger than previously computed we grow */
    if (n > max_n) {
        /* allocate new DP table */
        seq_t *new_dp = (seq_t*)calloc((size_t)(11 * (n + 1)), sizeof(seq_t));
        if (!new_dp)   /* allocation failure – return zero */
            return (seq_t)0;

        /* generate primes up to n */
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(n, &new_primes, &new_cnt);

        /* initialise DP: dp[0][0] = 1, all others are already 0 via calloc */
        new_dp[0] = (seq_t)1;

        /* DP: iterate over each prime, updating partitions */
        for (size_t pi = 0; pi < new_cnt; ++pi) {
            uint64_t p = new_primes[pi];
            for (int k = 1; k <= 10; ++k) {
                for (size_t s = p; s <= n; ++s) {
                    /* flatten indices */
                    size_t idx_cur  = (size_t)k * (n + 1) + s;
                    size_t idx_prev = (size_t)(k - 1) * (n + 1) + (s - p);
                    new_dp[idx_cur] += new_dp[idx_prev];
                }
            }
        }

        /* free old data */
        if (dp_arr)   free(dp_arr);
        if (primes)   free(primes);

        /* store new state */
        dp_arr = new_dp;
        primes = new_primes;
        prime_cnt = new_cnt;
        max_n = n;
    }

    /* result for required k = 10 */
    return dp_arr[10 * (max_n + 1) + n];
}
