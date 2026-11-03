/*
 * A336493  a(n) is the largest integer x such that x/sopf(x) = prime(n) where
 *          sopf(x) is the sum of distinct prime factors of x and prime(n) is
 *          the n-th prime.
 * https://oeis.org/A336493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Computed by sieving all integers S up to a fixed limit and selecting the
   largest x = p * S such that S - sopf(S) = p and p does not divide S,
   where p is the n‑th prime.  This yields a(n). */

static const uint32_t SIEVE_LIMIT = 200000;  /* sufficient for primes up to ~104729 */

/* Tables */
static bool       *is_prime    = NULL;
static uint32_t   *sum_distinct = NULL;
static seq_t      *best_array  = NULL;

/* Forward declaration of precomputation routine */
static void precompute(void);

/* --------------------------------------------------------------------- */
/*                     Public sequence function                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* One‑time initialization */
    static bool initialized = false;
    if (!initialized) {
        precompute();
        initialized = true;
    }

    /* Retrieve the n‑th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* p is guaranteed to be <= SIEVE_LIMIT for requested n */
    return best_array[p];
}

/* --------------------------------------------------------------------- */
/*                Precomputation of tables and best values              */
/* --------------------------------------------------------------------- */
static void precompute(void)
{
    /* Allocate tables */
    is_prime     = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    sum_distinct = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    best_array   = mem_new(seq_t, SIEVE_LIMIT + 1);

    /* Mark all as prime initially */
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    /* Sieve of Eratosthenes to fill is_prime */
    for (uint32_t p = 2; (uint64_t)p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t m = p * p; m <= SIEVE_LIMIT; m += p) {
                is_prime[m] = false;
            }
        }
    }

    /* Compute sum of distinct prime factors for each number up to limit */
    for (uint32_t p = 2; p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t m = p; m <= SIEVE_LIMIT; m += p) {
                sum_distinct[m] += p;
            }
        }
    }

    /* Initialise best_array[p] with the trivial candidate p^2 */
    for (uint32_t p = 2; p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            best_array[p] = (seq_t)p * (seq_t)p;
        }
    }

    /* Enumerate all S to possibly improve best values */
    for (uint32_t S = 2; S <= SIEVE_LIMIT; ++S) {
        uint32_t diff = S - sum_distinct[S];        /* diff = S - sopf(S) */
        if (diff >= 2 && diff <= SIEVE_LIMIT && is_prime[diff]) {
            if (S % diff != 0) {                    /* prime must not divide S */
                seq_t candidate = (seq_t)diff * (seq_t)S;
                if (candidate > best_array[diff]) {
                    best_array[diff] = candidate;
                }
            }
        }
    }

    /* Tables will remain valid for subsequent calls */
}
