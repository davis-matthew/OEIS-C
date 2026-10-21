/*
 * A280195  Expansion of 1/(1 - Sum_{k>=2} floor(1/omega(k))*x^k), where
 *          omega(k) is the number of distinct prime factors (A001221).
 * https://oeis.org/A280195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=2, floor(1/ω(k))=1} a(n-k), i.e., a(n) = Sum_{p^e
 *            <= n, p prime, e>=1} a(n-p^e), with a(0)=1, a(1)=0
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 99, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  =============================================================  */
/*  Prime sieve, prime‑power generation and dynamic programming   */
/*  =============================================================  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* auxiliary data */
static seq_t *dp          = NULL;   /* DP table */
static size_t dp_cap      = 0;      /* allocated size of dp   */
static idx_t  dp_size     = 0;      /* number of DP entries  */

/* primes list */
static uint64_t *prime    = NULL;
static size_t   prime_cnt = 0;

/* prime powers list (prime powers >=2) */
static uint64_t *prime_pow  = NULL;
static size_t   pow_cnt     = 0;

/* maximum n for which prime data are current */
static idx_t    max_n    = 0;

/* ------------------------------------------------------------- */
/* Compare two uint64_t values for qsort                         */
/* ------------------------------------------------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* ------------------------------------------------------------- */
/* Ensure dp has capacity at least need                         */
/* ------------------------------------------------------------- */
static void ensure_dp_cap(size_t need)
{
    if (dp_cap >= need) return;
    size_t new_cap = dp_cap > 0 ? dp_cap << 1 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *new_dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_dp) abort();
    if (dp) {
        memcpy(new_dp, dp, dp_size * sizeof(seq_t));
        free(dp);
    }
    dp = new_dp;
    dp_cap = new_cap;
}

/* ------------------------------------------------------------- */
/* Sieve of Eratosthenes – primes up to limit (inclusive)       */
/* ------------------------------------------------------------- */
static void compute_primes(idx_t limit)
{
    /* free old data */
    if (prime) { free(prime); prime = NULL; prime_cnt = 0; }

    if (limit < 2) return;

    bool *is_comp = (bool *)calloc((size_t)limit + 1, 1);
    if (!is_comp) abort();

    size_t est = limit / 2 + 1;            /* rough estimate size of primes */
    uint64_t *tmp = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!tmp) abort();

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (cnt < est) tmp[cnt++] = (uint64_t)i;
            if ((i * i) <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    prime = tmp;
    prime_cnt = cnt;
    free(is_comp);
}

/* ------------------------------------------------------------- */
/* Generate all prime powers <= limit (excluding 1), sorted.     */
/* ------------------------------------------------------------- */
static void compute_prime_powers(idx_t limit)
{
    /* free old data */
    if (prime_pow) { free(prime_pow); prime_pow = NULL; pow_cnt = 0; }

    if (limit < 2) return;

    /* first pass: estimate space needed */
    size_t est = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime[i];
        uint64_t x = p;
        while (x <= (uint64_t)limit) {
            ++est;
            if (x > (uint64_t)limit / p) break;
            x *= p;
        }
    }

    uint64_t *tmp = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!tmp) abort();

    /* second pass: store powers */
    size_t pos = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime[i];
        uint64_t x = p;
        while (x <= (uint64_t)limit) {
            tmp[pos++] = x;
            if (x > (uint64_t)limit / p) break;
            x *= p;
        }
    }

    /* sort ascending: now prime_pow holds sorted list of prime powers >=2 */
    qsort(tmp, pos, sizeof(uint64_t), cmp_uint64);

    prime_pow = tmp;
    pow_cnt   = pos;
}

/* ------------------------------------------------------------- */
/* Ensure data structures are ready for n                          */
/* ------------------------------------------------------------- */
static void ensure_data(idx_t n)
{
    if (dp_size > n) return;  /* already computed */

    /* allocate/extend dp */
    if (dp == NULL) {
        ensure_dp_cap(n + 1);
        dp[0] = (seq_t)1;               /* a(0)=1 */
        if (n >= 1) dp[1] = (seq_t)0;    /* a(1)=0 */
        dp_size = (n >= 1 ? 2 : 1);
    } else {
        ensure_dp_cap(n + 1);
    }

    /* rebuild primes and prime powers if necessary */
    if (max_n < n) {
        compute_primes(n);
        compute_prime_powers(n);
        max_n = n;
    }

    /* DP recurrence */
    for (idx_t i = dp_size; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < pow_cnt; ++k) {
            uint64_t p = prime_pow[k];
            if (p > (uint64_t)i) break;
            sum += dp[i - (idx_t)p];
        }
        dp[i] = sum;
    }
    dp_size = n + 1;
}

/* ------------------------------------------------------------- */
/* Public function                                            */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_data(n);
    return dp[n];
}
