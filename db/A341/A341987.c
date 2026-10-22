/*
 * A341987  Number of ways to write n as an ordered sum of 8 primes (counting
 *          1 as a prime).
 * https://oeis.org/A341987
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (x + Σ_{prime p} x^p)^8 i.e. number
 *            of ordered 8‑tuples of primes (with 1 treated as a prime) whose
 *            sum is n. Offset is 8
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- utility includes ---------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- prime sieve ---------- */
/* Return all primes <= limit in a newly allocated array.
   The length of the array is written to *cnt.
   The caller is responsible for freeing *primes. */
static void sieve_up_to(uint32_t limit, uint32_t **primes, size_t *cnt)
{
    if (limit < 2) {
        *primes = NULL;
        *cnt = 0;
        return;
    }
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime) return; /* allocation fail, return nothing */
    memset(is_prime, true, sz);
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
    /* count primes */
    size_t prime_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_cnt++;
    *primes = NULL;
    *cnt = prime_cnt;
    if (prime_cnt == 0) {
        free(is_prime);
        return;
    }
    uint32_t *p = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!p) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) p[idx++] = i;
    *primes = p;
    free(is_prime);
}

/* ---------- cached results ---------- */
static seq_t *cache = NULL;      /* a[0 .. cache_max] */
static size_t cache_cap = 0;     /* allocated size of cache */
static idx_t cache_max = 0;      /* highest n whose value is stored */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* values below offset are irrelevant */
    if (n < 8) return 0;

    /* compute up to n if necessary */
    if (cache == NULL || n > cache_max) {
        /* prepare primes up to n (including 1 as a token later) */
        uint32_t *prime_list = NULL;
        size_t prime_cnt = 0;
        sieve_up_to((uint32_t)n, &prime_list, &prime_cnt);

        /* number of tokens = primes + 1 (for 1) */
        size_t token_cnt = prime_cnt + 1;

        /* allocate token array: token[0] = 1, then primes */
        uint32_t *tokens = (uint32_t *)malloc(token_cnt * sizeof(uint32_t));
        if (!tokens) return 0; /* allocation failure */
        tokens[0] = 1;
        for (size_t i = 0; i < prime_cnt; ++i)
            tokens[i + 1] = prime_list[i];

        /* DP arrays */
        size_t N = (size_t)n + 1;
        seq_t *dp  = (seq_t *)calloc(N, sizeof(seq_t));
        seq_t *dpn = (seq_t *)calloc(N, sizeof(seq_t));
        if (!dp || !dpn) {
            free(tokens);
            if (prime_list) free(prime_list);
            return 0;
        }
        dp[0] = (seq_t)1; /* one way to reach sum 0 with 0 parts */

        for (int part = 0; part < 8; ++part) {
            /* clear destination */
            memset(dpn, 0, N * sizeof(seq_t));
            for (size_t s = 0; s <= n; ++s) {
                seq_t val = dp[s];
                if (!val) continue;
                for (size_t t = 0; t < token_cnt; ++t) {
                    uint32_t add = tokens[t];
                    size_t ns = s + add;
                    if (ns <= n) dpn[ns] += val;
                }
            }
            /* swap dp and dpn for next iteration */
            seq_t *tmp = dp;
            dp = dpn;
            dpn = tmp;
        }

        /* enlarge cache if necessary */
        if (cache_cap < N) {
            seq_t *new_cache = (seq_t *)malloc(N * sizeof(seq_t));
            if (!new_cache) {
                /* cleanup before exit */
                free(dp);
                free(dpn);
                free(tokens);
                if (prime_list) free(prime_list);
                return 0;
            }
            /* copy existing cached values if any */
            size_t copy_len = (cache_cap > 0) ? cache_cap : 0;
            if (copy_len > N) copy_len = N;
            if (cache && copy_len > 0)
                memcpy(new_cache, cache, copy_len * sizeof(seq_t));
            if (cache) free(cache);
            cache = new_cache;
            cache_cap = N;
        }

        /* copy computed values into cache */
        for (size_t i = 0; i < N; ++i)
            cache[i] = dp[i];
        cache_max = n;

        /* free temporaries */
        free(dpn);
        free(dp);
        free(tokens);
        if (prime_list) free(prime_list);
    }

    /* cache now contains value n */
    return cache[n];
}
