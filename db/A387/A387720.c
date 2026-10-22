/*
 * A387720  Numbers k for which A107758(k) < 2*k, where A107758 is sigma+,
 *          multiplicative function with a(p^e) = 1+sigma(p^e).
 * https://oeis.org/A387720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 9183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387720: numbers k with sigma+(k) < 2*k.  sigma+ is multiplicative
   with a(p^e) = 1 + sigma(p^e).  The implementation finds successive odd
   integers and tests the inequality by prime factorisation. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* Precompute primes up to a fixed limit (1 000 000). */
static void init_primes(void)
{
    if (primes != NULL) return;
    const uint32_t LIMIT = 1000000U;          /* enough for the required range */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes first to know array size */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[primes_cnt++] = i;

    free(is_prime);
}

/* Return true iff sigma+(k) < 2*k.  k is odd and >0. */
static bool satisfies(uint64_t k)
{
    if (k == 1) return true;           /* sigma+(1) = 1 < 2 */

    uint64_t n = k;
    seq_t prod = 1;
    const seq_t twice_n = (seq_t)2 * (seq_t)k;

    for (size_t i = 0; i < primes_cnt &&
                    (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            /* Determine exponent e */
            uint32_t e = 0;
            while (n % p == 0) { n /= p; ++e; }

            /* Compute sigma(p^e) = 1 + p + ... + p^e */
            uint64_t sum = 1;
            uint64_t p_pow = p;
            for (uint32_t j = 1; j <= e; ++j) {
                sum += p_pow;
                p_pow *= p;
            }

            /* a(p^e) = 1 + sigma(p^e) = sum + 1 */
            prod *= (seq_t)(sum + 1);
            if (prod >= twice_n) return false;
        }
    }

    if (n > 1) {          /* remaining prime factor (exponent 1) */
        /* sigma(p)=1+p, a(p)=2+p */
        prod *= (seq_t)(2 + n);
        if (prod >= twice_n) return false;
    }

    return prod < twice_n;
}

/* Cache of already computed terms. */
static seq_t *terms_cache = NULL;
static size_t cache_size = 0;     /* number of stored terms */

/* Next odd integer to be tested. Even numbers are never candidates. */
static uint64_t next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for zero index */

    if (terms_cache == NULL) {           /* initialisation */
        terms_cache = NULL;
        cache_size = 0;
        init_primes();
    }

    /* Generate terms until the n‑th term is available. */
    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate;
        if (satisfies(cand)) {
            /* Store the found term. */
            seq_t *tmp = (seq_t *)realloc(terms_cache,
                            (cache_size + 1) * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure – should not happen */
            terms_cache = tmp;
            terms_cache[cache_size] = (seq_t)cand;
            ++cache_size;
        }
        next_candidate += 2;             /* next odd number */
    }

    return terms_cache[n - 1];
}
