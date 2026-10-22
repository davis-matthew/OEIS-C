/*
 * A321262  Expansion of 1/(1 - Sum_{k>=1} k*x^(2*k)/(1 - x^k)).
 * https://oeis.org/A321262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321262 – Expansion of 1/(1 - Sum_{k>=1} k*x^(2*k)/(1 - x^k)).
 *
 *  The sequence satisfies the recurrence
 *      a(0) = 1
 *      a(n) = Σ_{k=1..n} A001065(k) * a(n-k)
 *
 *  where A001065(k) = Σ_{d|k} d  - k  (i.e. sigma(k)-k).
 *
 *  The implementation uses memoised convolution and a simple
 *  divisor‐sum sieve.  It works for any unsigned integer type
 *  provided by the build system (seq_t and idx_t).
 */

#include <stdlib.h>

/* ---------- Cached data ---------------------------------------------- */
static seq_t *a_cache      = NULL;   /* a[0..a_last]            */
static size_t a_cap        = 0;       /* allocated size of a_cache   */
static idx_t  a_last       = 0;       /* last index computed for a  */

static seq_t *sigma_cache  = NULL;   /* sigma_cache[0..sigma_last] */
static size_t sigma_cap    = 0;       /* allocated size of sigma_cache */
static idx_t  sigma_last   = 0;       /* largest index with computed sigma */

/* ---------- Initialize a_cache with a(0) ------------------------------ */
static void init_a_cache_if_needed(void)
{
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = (seq_t)1;
        a_cap = 1;
        a_last = 0;
    }
}

/* ---------- Recompute sigma(k) up to 'limit' -------------------------- */
static void recompute_sigma(idx_t limit)
{
    /* Ensure enough space */
    if ((size_t)(limit + 1) > sigma_cap) {
        seq_t *tmp = (seq_t *)realloc(sigma_cache, (size_t)(limit + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        sigma_cache = tmp;
        sigma_cap   = (size_t)(limit + 1);
    }

    /* Zero the array */
    for (size_t i = 0; i <= (size_t)limit; ++i)
        sigma_cache[i] = (seq_t)0;

    /* Sieve to compute sigma(k) */
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j <= limit; j += i)
            sigma_cache[j] += add;
    }

    sigma_last = limit;
}

/* ---------- Ensure a(n) is cached ------------------------------------ */
static void ensure_a_cached(idx_t n)
{
    init_a_cache_if_needed();

    if (n <= a_last) return;         /* already computed */

    /* Make sure sigma values are available up to n */
    if (n > sigma_last)
        recompute_sigma(n);

    /* Allocate more space for a_cache if necessary */
    if ((size_t)(n + 1) > a_cap) {
        seq_t *tmp = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        a_cap   = (size_t)(n + 1);
    }

    /* Compute new terms of a up to n */
    for (idx_t m = a_last + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t coeff = sigma_cache[k] - (seq_t)k;   /* A001065(k) */
            if (coeff == (seq_t)0) continue;
            sum += a_cache[m - k] * coeff;
        }
        a_cache[m] = sum;
    }

    a_last = n;
}

/* ---------- Public function ------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_a_cached(n);
    return a_cache[n];
}
