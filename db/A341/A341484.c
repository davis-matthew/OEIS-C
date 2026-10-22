/*
 * A341484  Number of ways to write n as an ordered sum of 7 nonprime numbers.
 * https://oeis.org/A341484
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered sum of 7 nonprime numbers.
 *            Implementation uses inclusion–exclusion: a(n) = Σ_{k=0}^7 (-1)^k
 *            * C(7,k) * Σ_{t=7-k}^{n} C(t-1, 6-k) * P_k(n-t) where P_k(s) =
 *            number of ordered k‑tuples of primes summing to s
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"     /* alloc_seq(size_t) */
#include "../utils/free_seq_array.h"/* free_seq_array(seq_t*) */

static seq_t *dp[8] = {NULL};          /* dp[k][s] = #ordered k‑prime tuples summing to s */
static size_t dp_cap = 0;              /* current allocated size (max sum) + 1 */
static uint32_t *prime_list = NULL;    /* array of primes up to current max */
static size_t prime_count = 0;

/* Ensure that dp tables and prime list are computed up to at least n. */
static void ensure_data(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= dp_cap) return;

    /* free old arrays */
    for (size_t k = 0; k < 8; ++k) {
        if (dp[k]) free_seq_array(dp[k]);
    }
    if (prime_list) free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    /* sieve primes up to n */
    bool *is_prime = (bool *)malloc(need * sizeof(bool));
    for (size_t i = 0; i < need; ++i) is_prime[i] = true;
    if (need > 0) is_prime[0] = false;
    if (need > 1) is_prime[1] = false;
    for (size_t i = 2; i * i < need; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < need; j += i) is_prime[j] = false;
        }
    }
    /* count primes */
    for (size_t i = 2; i < need; ++i) if (is_prime[i]) ++prime_count;
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    size_t pos = 0;
    for (size_t i = 2; i < need; ++i) if (is_prime[i]) prime_list[pos++] = (uint32_t)i;
    free(is_prime);

    /* allocate dp[0] */
    dp[0] = alloc_seq(need);
    for (size_t i = 0; i < need; ++i) dp[0][i] = 0;
    dp[0][0] = 1;
    seq_t *prev = dp[0];

    /* build dp[1] … dp[7] */
    for (size_t k = 1; k <= 7; ++k) {
        seq_t *curr = alloc_seq(need);
        for (size_t i = 0; i < need; ++i) curr[i] = 0;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            size_t p = prime_list[pi];
            for (size_t s = p; s < need; ++s) {
                curr[s] += prev[s - p];
            }
        }
        dp[k] = curr;
        prev = curr;
    }

    dp_cap = need;
}

/* Binomial coefficient C(n,r) for r<=7 (small, returns seq_t) */
static seq_t binom_small(size_t n, size_t r)
{
    seq_t res = 1;
    for (size_t i = 1; i <= r; ++i) {
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;
    }
    return res;
}

/* nCr for 7 choose k (k<=7) */
static seq_t comb_7_k(size_t k)
{
    return binom_small(7, k);
}

seq_t a(idx_t n)
{
    if ((idx_t)7 <= n && n < 7) return 0;  /* safe guard, though unused */

    /* Ensure tables up to n. */
    ensure_data(n);

    seq_t result = 0;

    for (size_t k = 0; k <= 7; ++k) {
        size_t m = 7 - k;                /* number of nonprime factors */
        seq_t sum = 0;

        if (m == 0) {                    /* all parts are primes */
            if ((size_t)n < dp_cap)
                sum = dp[7][(size_t)n];
        } else {
            for (size_t t = m; t <= (size_t)n; ++t) {
                size_t s = (size_t)n - t;   /* remaining sum for primes */
                if (s >= dp_cap) continue;
                seq_t ways_prime = dp[k][s];
                if (ways_prime == 0) continue;

                /* Binomial coefficient C(t-1, m-1) */
                seq_t bin = 1;
                for (size_t i = 1; i <= m - 1; ++i) {
                    bin = bin * (seq_t)(t - i);
                    bin = bin / (seq_t)i;
                }
                sum += bin * ways_prime;
            }
        }

        sum *= comb_7_k(k);   /* multiply by C(7,k) */

        if (k % 2 == 1)
            result -= sum;
        else
            result += sum;
    }

    return result;
}
