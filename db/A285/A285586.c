/*
 * A285586  Complete list of numbers n for which there exists no prime number
 *          between n and 9n/8 inclusive.
 * https://oeis.org/A285586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285586: numbers n for which there is no prime between
 * n and 9n/8 inclusive.  The function generates the entire
 * finite sequence once and returns the nth (1‑based) term.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SIEVE_LIMIT 1000000U          /* upper bound for searching n */

static seq_t *terms = NULL;          /* array of sequence values   */
static size_t term_cnt = 0;          /* how many terms were found  */
static bool initialized = false;    /* ensure one-time generation */

/* Build a list of primes up to SIEVE_LIMIT using a simple sieve.  */
static void build_primes(uint64_t limit, uint64_t **out, size_t *cnt)
{
    /* Allocate and initialize boolean array for the sieve.  */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime) exit(1);           /* allocation failed */

    for (size_t i = 2; i <= (size_t)limit; ++i) is_prime[i] = true;

    for (size_t i = 2; (size_t)i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes first. */
    size_t prime_cap = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++prime_cap;

    uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        exit(1);
    }

    size_t pidx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) primes[pidx++] = (uint64_t)i;
    }

    free(is_prime);

    *out = primes;
    *cnt = prime_cap;
}

/* Generate all terms of the sequence within the search limit. */
static void init(void)
{
    /* Obtain primes up to SIEVE_LIMIT. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(SIEVE_LIMIT, &primes, &prime_cnt);

    /* Prepare a dynamic array for the terms. */
    size_t cap = 32;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!terms) {
        free(primes);
        exit(1);
    }

    /* Scan every integer m in [1, SIEVE_LIMIT].  */
    for (uint64_t m = 1; m <= SIEVE_LIMIT; ++m) {
        /* If m itself is prime, it cannot satisfy the condition. */
        if (m <= SIEVE_LIMIT && primes[0] <= m) {       /* handle quickly */
            /* binary search to detect if m is prime. */
            size_t lo = 0, hi = prime_cnt;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (primes[mid] < m)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            if (lo < prime_cnt && primes[lo] == m) continue;
        }

        /* Find the first prime greater than m. */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] <= m)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo == prime_cnt) continue;   /* no prime after m within sieve limit */

        uint64_t next_prime = primes[lo];
        uint64_t xmax = (9ULL * m) / 8ULL;   /* floor(9m/8) */

        if (next_prime > xmax) {          /* condition satisfied */
            if (term_cnt == cap) {         /* grow the terms array */
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms) {
                    free(primes);
                    exit(1);
                }
            }
            terms[term_cnt++] = (seq_t)m;
        }
    }

    free(primes);
    initialized = true;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if ((idx_t)n > term_cnt) return (seq_t)0; /* undefined for out‑of‑range */
    return terms[(size_t)(n - 1)];
}
