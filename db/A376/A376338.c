/*
 * A376338  Numbers k such that phi(k)/2 - 1 = phi(k + 1) where phi = A000010.
 * https://oeis.org/A376338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for OEIS A376338 (offset 1).
    The sequence consists of the number 49 and all primes of the form 4p‑1
    with p an odd prime.  The function lazily generates terms on demand
    using a sieve of primes up to a fixed limit.  It is robust to any
    build where seq_t and idx_t are defined externally. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static const uint32_t PRIME_LIMIT = 5000000;   /* should cover ≥10 000 terms */

static uint8_t *prime_sieve  = NULL;
static uint32_t primes[400000];
static size_t   primes_cnt   = 0;

static seq_t  *terms      = NULL;
static size_t terms_size  = 0;
static size_t terms_cap   = 0;

static size_t next_prime_idx = 0;
static bool   inserted49     = false;

/* Build the prime sieve and prime list once */
static void
ensure_primes(void)
{
    if (prime_sieve != NULL)
        return;

    size_t size = (size_t)PRIME_LIMIT + 1;
    prime_sieve = (uint8_t *)malloc(size);
    if (!prime_sieve)
        exit(1);
    memset(prime_sieve, 1, size);
    prime_sieve[0] = prime_sieve[1] = 0;

    for (size_t i = 2; i * i <= (size_t)PRIME_LIMIT; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= (size_t)PRIME_LIMIT; j += i)
                prime_sieve[j] = 0;
        }
    }

    primes_cnt = 0;
    for (size_t i = 2; i <= (size_t)PRIME_LIMIT; ++i) {
        if (prime_sieve[i])
            primes[primes_cnt++] = (uint32_t)i;
    }
}

/* Ensure the terms buffer can hold at least `need` elements */
static void
ensure_terms_cap(size_t need)
{
    if (terms_cap >= need)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);
    terms = new_arr;
    terms_cap = new_cap;
}

/* Generate terms until we have at least `n_needed` values */
static void
generate_terms(idx_t n_needed)
{
    if (terms_size >= (size_t)n_needed)
        return;

    ensure_primes();

    while (terms_size < (size_t)n_needed) {
        /* If we ran out of precomputed primes, enlarge the limit (unnecessary for n≤10000). */
        if (next_prime_idx >= primes_cnt)
            break;

        uint32_t p = primes[next_prime_idx++];
        if (p == 2)   /* skip the even prime */
            continue;

        seq_t k = (seq_t)4 * (seq_t)p - (seq_t)1;

        /* Handle values < 49 (they appear before the special 49). */
        if (k <= (seq_t)48) {
            if (is_prime_small(k)) {
                ensure_terms_cap(terms_size + 1);
                terms[terms_size++] = k;
            }
            continue;
        }

        /* Skip the non‑primality case k == 49 (does not occur). */
        if (k == (seq_t)49)
            continue;

        /* Insert the special number 49 just before the first k > 49. */
        if (!inserted49) {
            ensure_terms_cap(terms_size + 1);
            terms[terms_size++] = (seq_t)49;
            inserted49 = true;

            /* Re‑process this prime, as we have not yet handled its k. */
            if (next_prime_idx > 0)
                --next_prime_idx;
            continue;
        }

        /* Normal case: k > 49 and 49 already inserted. */
        if (is_prime_small(k)) {
            ensure_terms_cap(terms_size + 1);
            terms[terms_size++] = k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for offset 1; guard against underflow */

    generate_terms(n);
    return terms[(size_t)n - 1];
}
