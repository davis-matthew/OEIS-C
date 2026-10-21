/*
 * A285692  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of 7 primes.
 * https://oeis.org/A285692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A285692
 * a(1) = 2
 * a(n+1) = smallest prime > a(n) such that a(n+1)-a(n) is the product
 *          of exactly 7 primes (counted with multiplicity).
 * This implementation computes the sequence on demand, memoising
 * already‑computed terms to support arbitrary order of calls.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Prime generation (sieve) – used for trial division in next_prime()   */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list = NULL; /* primes up to LIMIT */
static size_t     prime_count = 0;
static const size_t PRIME_SIEVE_LIMIT = 2000000ULL; /* two million */

static void init_primes(void)
{
    if (prime_count > 0) return; /* already initialised */

    /* ---------- sieve of Eratosthenes ---------- */
    bool *is_composite = (bool*)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) return;    /* allocation failure – unlikely */

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_composite[j] = true;

    /* count primes */
    prime_count = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_composite[i]) ++prime_count;

    /* allocate and store primes */
    prime_list = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_composite[i]) prime_list[idx++] = i;

    free(is_composite);
}

/* --------------------------------------------------------------------- */
/* Utility: smallest prime > x                                         */
/* --------------------------------------------------------------------- */
static uint64_t next_prime(uint64_t x)
{
    if (x < 2) return 2;
    uint64_t cand = x + 1;
    if (cand % 2 == 0) /* even */
        ++cand;
    for (;; cand += 2) {
        bool prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= cand; ++i)
            if (cand % prime_list[i] == 0) { prime = false; break; }
        if (prime) return cand;
    }
}

/* --------------------------------------------------------------------- */
/* Count prime factors (with multiplicity) of a 64‑bit integer           */
/* --------------------------------------------------------------------- */
static size_t count_prime_factors(uint64_t n)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i) {
        while (n % prime_list[i] == 0) { n /= prime_list[i]; ++cnt; }
    }
    if (n > 1) ++cnt;   /* remaining prime factor > sqrt(original n) */
    return cnt;
}

/* --------------------------------------------------------------------- */
/* Memoised sequence storage                                            */
/* --------------------------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static idx_t  max_stored = 0;  /* number of terms stored (1‑based) */
static seq_t  current_value = 0; /* holds a(max_stored) */

/* --------------------------------------------------------------------- */
/* Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* defined as zero for safety */

    /* Expand cache if needed */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory – unlikely in tests */
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* Initialise on first use */
    if (max_stored == 0) {
        init_primes();
        current_value = (seq_t)2;
        cache[0] = current_value;
        max_stored  = 1;
    }

    /* Compute terms up to n (inclusive) */
    for (idx_t i = max_stored + 1; i <= n; ++i) {
        uint64_t cur = (uint64_t)current_value;

        /* Find next prime that satisfies the condition */
        uint64_t candidate = next_prime(cur);
        for (;;) {
            uint64_t diff = candidate - cur;
            if (count_prime_factors(diff) == 7) {
                current_value = (seq_t)candidate;
                break;
            }
            candidate = next_prime(candidate);
        }

        cache[i - 1] = current_value;
    }

    max_stored = n;
    return cache[n - 1];
}
