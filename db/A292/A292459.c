/*
 * A292459  a(n) is the smallest number whose factorial has at least n!
 *          divisors.
 * https://oeis.org/A292459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A292459
 *
 *  a(n) = the smallest integer m such that τ(m!) ≥ n!
 *  where τ(k) is the number of divisors of k.
 *
 *  The code computes τ(m!) by multiplying (e_p + 1) for all
 *  primes p ≤ m, where e_p = ∑_{k≥1} ⌊m/p^k⌋.
 *  The comparison with n! is performed on logarithms:
 *
 *      log τ(m!)  ≥  log n!
 *
 *  Pre‑computed tables:
 *      * primes up to LIMIT (2·10⁷) via a simple sieve
 *      * cumulative logarithms of factorials up to MAX_N (1 000 000)
 *
 *  The function is memoised; the array `cache` stores the
 *  value of a(n) for each n that has already been requested.
 *
 *  The algorithm for a new n:
 *      1.  Compute log n! from the pre‑computed table.
 *      2.  Determine an upper bound M such that τ(M!) ≥ n!
 *          by repeatedly doubling an initial guess.
 *      3.  Binary search in [0, M] for the smallest m with
 *          τ(m!) ≥ n!.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* maximum n we accept in this implementation */
#define MAX_N 1000000UL
/* upper bound for m we will generate primes for */
#define PRIME_LIMIT 20000000UL

static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* logarithms of factorials: log_fact[i] = log(i!) */
static long double *log_fact = NULL;

/* memoisation cache for a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* ------------------------------------------------------------------
 *  Helper: Sieve of Eratosthenes up to PRIME_LIMIT
 * ------------------------------------------------------------------*/
static void
init_primes(void)
{
    if (primes != NULL) return;

    size_t limit = PRIME_LIMIT;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    primes = (uint64_t *)malloc((limit / 10) * sizeof(uint64_t)); /* rough over‑estimate */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = true;
        }
    }

    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (prime_count == (limit / 10)) {           /* resize if needed */
                prime_count = (prime_count + 1) * 2;
                primes = (uint64_t *)realloc(primes, prime_count * sizeof(uint64_t));
            }
            primes[prime_count++] = i;
        }
    }

    free(sieve);
}

/* ------------------------------------------------------------------
 *  Helper: Precompute logarithms of factorials up to MAX_N
 * ------------------------------------------------------------------*/
static void
init_log_fact(void)
{
    if (log_fact != NULL) return;

    log_fact = (long double *)malloc((MAX_N + 1) * sizeof(long double));
    log_fact[0] = 0.0L;
    log_fact[1] = 0.0L;
    for (size_t i = 2; i <= MAX_N; ++i)
        log_fact[i] = log_fact[i - 1] + logl((long double)i);
}

/* ------------------------------------------------------------------
 *  Helper: τ(m!) computed via logarithm
 * ------------------------------------------------------------------*/
static long double
log_divisor_count_factorial(uint64_t m)
{
    long double sum = 0.0L;
    for (size_t i = 0; i < prime_count && primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        uint64_t e = 0;
        uint64_t pk = p;
        while (pk <= m) {
            e += m / pk;
            if (pk > m / p) break;
            pk *= p;
        }
        sum += logl((long double)(e + 1));
    }
    return sum;
}

/* ------------------------------------------------------------------
 *  Helper: compute log(n!) using precomputed table or summation
 * ------------------------------------------------------------------*/
static long double
log_factorial(idx_t n)
{
    if (n <= MAX_N) {
        return log_fact[n];
    }
    /* if n > MAX_N, compute on the fly */
    long double sum = log_fact[MAX_N];
    for (idx_t i = MAX_N + 1; i <= n; ++i)
        sum += logl((long double)i);
    return sum;
}

/* ------------------------------------------------------------------
 *  Main function: a(n)
 * ------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise helpers on first use */
    init_primes();
    init_log_fact();

    if (n < 2) return (seq_t)0;

    /* grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise new entries */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)(-1);
        cache_cap = new_cap;
    }

    /* return memoised value if present */
    if (cache[(size_t)n] != (seq_t)(-1))
        return cache[(size_t)n];

    /* compute log(n!) */
    long double ln_nfac = log_factorial(n);

    /* find an upper bound M with τ(M!) ≥ n! by doubling */
    uint64_t lo = 1, hi = 2;
    while (true) {
        long double ln_tau_hi = log_divisor_count_factorial(hi);
        if (ln_tau_hi >= ln_nfac) break;
        lo = hi;
        hi <<= 1;     /* double */
    }

    /* binary search for minimal m */
    uint64_t ans = hi;
    while (lo <= hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        long double ln_tau_mid = log_divisor_count_factorial(mid);
        if (ln_tau_mid >= ln_nfac) {
            ans = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    cache[(size_t)n] = (seq_t)ans;
    return ans;
}
