/*
 * A286042  Largest prime factor of A285993(n), the largest odd abundant
 *          number (A005231) equal to the product of n consecutive primes.
 * https://oeis.org/A286042
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest prime p such that the product of n *
 *            consecutive odd primes ending at p is an odd abundant number. *
 *            This is equivalent to the largest prime p for which * ∏_{i} (1 +
 *            1/p_i) > 2 * for the window of n consecutive odd primes ending
 *            with p. * * The implementation precomputes odd p...
 * Range:     u8 n <= 49, u16 n <= 6491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static uint32_t *primes = NULL;        /* odd primes only (2 is omitted) */
static size_t prime_count = 0;
static long double *ratio_fact = NULL; /* 1 + 1/p for each odd prime */

/* cache for previously computed a(n) values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;          /* number of cached indices */
static idx_t cached_up_to = 4;        /* largest n cached (offset 5) */

/* ------------------------------------------------------------------ */
/* Sieve helper: generate odd primes up to LIMIT and build ratio array. */
static void init_primes(void)
{
    const size_t LIMIT = 500000;              /* sufficient for n <= 10000 */
    bool *is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(1);                  /* allocation failure */

    /* basic sieve */
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* count odd primes (excluding 2) */
    size_t cnt = 0;
    for (size_t i = 3; i <= LIMIT; i += 2) if (is_prime[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) exit(1);
    ratio_fact = (long double*)malloc(cnt * sizeof(long double));
    if (!ratio_fact) exit(1);

    size_t idx = 0;
    for (size_t i = 3; i <= LIMIT; i += 2) {
        if (is_prime[i]) {
            primes[idx] = (uint32_t)i;
            ratio_fact[idx] = 1.0L + 1.0L / (long double)i;
            ++idx;
        }
    }
    prime_count = cnt;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Ensure cache large enough */
static void ensure_cache_size(idx_t needed)
{
    if (needed + 1 <= cache_size) return;
    size_t new_size = needed + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);
    /* initialise new entries to zero */
    for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

/* ------------------------------------------------------------------ */
/* Compute a(n) for  n >= 5 */
static seq_t compute_a(idx_t n)
{
    size_t N = (size_t)n;         /* work with size_t for indices */

    long double prod = 1.0L;

    /* initial product for first N odd primes */
    for (size_t j = 0; j < N; ++j) prod *= ratio_fact[j];

    size_t best_idx = 0;
    if (prod > 2.0L) best_idx = N - 1;

    /* slide the window until product <= 2 (then stop) */
    for (size_t k = N; k < prime_count; ++k) {
        prod = prod * ratio_fact[k] / ratio_fact[k - N];
        if (prod > 2.0L) {
            best_idx = k;
        } else {
            break;                 /* further windows will be even smaller */
        }
    }

    return (seq_t)primes[best_idx];
}

/* ------------------------------------------------------------------ */
/* User function */
seq_t a(idx_t n)
{
    if (n < 5) return 0;           /* undefined below offset, return 0 */

    if (!initialized) {
        init_primes();
        initialized = true;
    }

    ensure_cache_size(n);

    /* compute missing values up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        cache[i] = compute_a(i);
    }
    cached_up_to = n;

    return cache[n];
}
