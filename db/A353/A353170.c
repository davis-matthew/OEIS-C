/*
 * A353170  Expansion of Product_{k>=1} (1 - x^k)^prime(k+1).
 * https://oeis.org/A353170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 – x^k)^{prime(k+1)}. The
 *            function evaluates this coefficient by iteratively convolving
 *            the generating series with the polynomial (1 – x^k)^p where p =
 *            prime(k+1). An on‑demand sieve generates the first primes as
 *            needed. The implementation is fully static an...
 * Range:     i8 n <= 9, i16 n <= 28, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

/*------------------------------------------------------------------------------*/
/*                                          Prime generation                    */
/*------------------------------------------------------------------------------*/
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;     /* number of primes stored */
static size_t   prime_limit = 0;     /* sieve size used so far */

/* generate primes up to 'limit' (inclusive) and store them in prime_list */
static void generate_primes_upto(size_t limit)
{
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;  /* out of memory – unlikely in contest use */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes first */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *tmp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!tmp) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) tmp[idx++] = (uint32_t)i;

    free(is_prime);
    free(prime_list);
    prime_list = tmp;
    prime_cnt   = cnt;
    prime_limit = limit;
}

/* ensure at least (need) primes are available */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;
    /* a rough upper bound for the nth prime (n≥6) is n(ln n + ln ln n);
       here we use a simple safe multiplier */
    size_t limit = need * 20 + 100;
    generate_primes_upto(limit);
}

/*------------------------------------------------------------------------------*/
/*                                  Sequence evaluation                           */
/*------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static buffers for DP */
    static seq_t *dp      = NULL;
    static size_t dp_cap  = 0;
    static seq_t *tmp_arr = NULL;
    static size_t tmp_cap = 0;

    size_t N = (size_t)n;            /* array size needed */
    if (dp_cap <= N) {
        dp_cap  = N + 1;
        dp      = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
    }
    if (tmp_cap <= N) {
        tmp_cap = N + 1;
        tmp_arr = (seq_t *)realloc(tmp_arr, tmp_cap * sizeof(seq_t));
    }

    /* initialise series: a(0) = 1, others 0 */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) dp[i] = (seq_t)0;

    /* we need primes up to index n+1 (prime(k+1) for k = 1..n) */
    ensure_primes(n + 1);

    /* main convolution loop */
    for (idx_t k = 1; k <= n; ++k) {
        /* prime of index k+1: prime_list[k] (prime_list[0] = 2) */
        idx_t p = (idx_t)prime_list[(size_t)k];

        /* how many terms of (1 - x^k)^p affect degree n? */
        idx_t max_m = n / k;
        if (max_m > p) max_m = p;

        /* start with current series in tmp_arr */
        for (size_t i = 0; i <= N; ++i) tmp_arr[i] = dp[i];

        /* binomial coefficient iteratively */
        seq_t binom = (seq_t)1;   /* C(p,0) */
        for (idx_t m = 1; m <= max_m; ++m) {
            binom = binom * (seq_t)(p - m + 1);
            binom = binom / (seq_t)m;        /* exact division */

            seq_t coeff = (m & 1) ? -binom : binom;
            idx_t shift = k * m;             /* shift amount */

            for (size_t j = 0; j + shift <= N; ++j)
                tmp_arr[j + shift] += dp[j] * coeff;
        }

        /* prepare for next k: new series is in tmp_arr */
        seq_t *swap = dp;
        dp = tmp_arr;
        tmp_arr = swap;
    }

    return dp[N];
}
