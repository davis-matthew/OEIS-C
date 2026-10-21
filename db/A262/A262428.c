/*
 * A262428  Concatenate the n-th prime with the n-th semiprime.
 * https://oeis.org/A262428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenate(n-th prime, n-th semiprime)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Helper: concatenate two unsigned integers a | b                           */
/* --------------------------------------------------------------------- */
static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t pow10 = 1;
    idx_t t = b;
    while (t > 0) {
        pow10 *= 10;
        t /= 10;
    }
    return (seq_t)a * pow10 + b;
}

/* --------------------------------------------------------------------- */
/* Cached data for primes and semiprimes                                   */
/* --------------------------------------------------------------------- */
static uint64_t *g_cached_primes = NULL;   /* up to g_prime_cnt */
static size_t   g_prime_cnt    = 0;
static idx_t   *g_cached_semiprimes = NULL;   /* up to g_semiprime_cnt */
static size_t   g_semiprime_cnt = 0;
static idx_t   *g_spf           = NULL;      /* smallest prime factor */
static idx_t   g_spf_size      = 0;

/* --------------------------------------------------------------------- */
/* Recompute caches when requested n is larger than the current cache    */
/* --------------------------------------------------------------------- */
static void recompute_cache(idx_t n)
{
    if (n <= 0) return;

    /* --- Choose an upper bound for the n-th prime -------------------- */
    /* Empirically, p_n < 20*n + 100 for n >= 1 (sufficient for n <= 10k) */
    idx_t limit_prime = n * 20 + 100;
    /* Upper bound for the n-th semiprime; we use twice the prime limit.  */
    idx_t limit_semiprime = limit_prime * 2;

    /* --- Allocate/spare the SPF array -------------------------------- */
    if (limit_semiprime > g_spf_size) {
        free(g_spf);
        g_spf = (idx_t *)malloc((limit_semiprime + 1) * sizeof(idx_t));
        if (!g_spf) exit(EXIT_FAILURE);
        g_spf_size = limit_semiprime + 1;
    }
    for (idx_t i = 0; i <= limit_semiprime; ++i) g_spf[i] = 0;

    /* --- Sieve to fill SPF ------------------------------------------- */
    idx_t i;
    for (i = 2; i <= limit_semiprime; ++i) {
        if (g_spf[i] == 0) {
            g_spf[i] = i;                     /* i is prime */
            if (i > limit_semiprime / i) continue;
            for (idx_t j = i * i; j <= limit_semiprime; j += i) {
                if (g_spf[j] == 0) g_spf[j] = i;
            }
        }
    }

    /* --- Collect primes up to limit_prime ----------------------------- */
    size_t prime_capacity = 0;
    g_prime_cnt = 0;
    for (i = 2; i <= limit_prime; ++i) {
        if (g_spf[i] == i) {
            if (g_prime_cnt == prime_capacity) {
                prime_capacity = prime_capacity ? prime_capacity * 2 : 128;
                uint64_t *tmp = (uint64_t *)realloc(g_cached_primes,
                                                    prime_capacity * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                g_cached_primes = tmp;
            }
            g_cached_primes[g_prime_cnt++] = (uint64_t)i;
        }
    }

    /* --- Collect semiprimes up to limit_semiprime -------------------- */
    size_t semiprime_capacity = 0;
    g_semiprime_cnt = 0;
    for (i = 2; i <= limit_semiprime; ++i) {
        idx_t t = i;
        unsigned factor_count = 0;
        while (t > 1 && factor_count <= 2) {
            ++factor_count;
            t /= g_spf[t];
        }
        if (factor_count == 2) {
            if (g_semiprime_cnt == semiprime_capacity) {
                semiprime_capacity = semiprime_capacity ? semiprime_capacity * 2 : 128;
                idx_t *tmp = (idx_t *)realloc(g_cached_semiprimes,
                                              semiprime_capacity * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                g_cached_semiprimes = tmp;
            }
            g_cached_semiprimes[g_semiprime_cnt++] = i;
            if (g_semiprime_cnt >= (size_t)n) break;  /* enough */
        }
    }

    /* --- Truncate excess space (optional) ----------------------------- */
    if (g_prime_cnt < prime_capacity) {
        uint64_t *tmp = (uint64_t *)realloc(g_cached_primes, g_prime_cnt * sizeof(uint64_t));
        if (tmp) g_cached_primes = tmp;
    }
    if (g_semiprime_cnt < semiprime_capacity) {
        idx_t *tmp = (idx_t *)realloc(g_cached_semiprimes, g_semiprime_cnt * sizeof(idx_t));
        if (tmp) g_cached_semiprimes = tmp;
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; behaviour undefined for 0 */

    /* Ensure caches contain at least n elements */
    if (n > g_prime_cnt || n > g_semiprime_cnt) {
        recompute_cache(n);
    }
    return concat_parts(g_cached_primes[n - 1], g_cached_semiprimes[n - 1]);
}
