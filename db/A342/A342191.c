/*
 * A342191  Numbers with no adjacent prime indices having quotient < 1/2.
 * https://oeis.org/A342191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose prime indices sorted have no adjacent pair with ratio > 2 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global state for memoization and prime data */
static seq_t *terms = NULL;
static size_t terms_size   = 0;
static size_t terms_capacity = 0;

static uint32_t *spf = NULL;          /* smallest prime factor array */
static uint32_t *prime_index = NULL;  /* prime → index */
static size_t prime_limit = 0;        /* current maximum index spf covers */

static uint32_t *primes = NULL;     /* list of primes */
static size_t primes_size   = 0;
static size_t primes_capacity = 0;

/* next natural number to be tested */
static idx_t next_scan = 1;

/* whether the static data has been initialized */
static bool initialized = false;

/* ------------------------------------------------------------ */
/* Ensure that primes, spf and prime_index are available up to
   at least 'limit'. Rebuilds the tables if necessary. */
static void init_primes(idx_t limit)
{
    size_t size = (size_t)limit + 1;
    uint32_t *new_spf = (uint32_t*)malloc(size * sizeof(uint32_t));
    uint32_t *new_prime_index = (uint32_t*)malloc(size * sizeof(uint32_t));

    /* initialise */
    for (size_t i = 0; i < size; ++i) {
        new_spf[i] = 0;
        new_prime_index[i] = 0;
    }

    /* ensure primes array capacity */
    if (primes_capacity == 0) {
        primes_capacity = 64;
        primes = (uint32_t*)malloc(primes_capacity * sizeof(uint32_t));
    }

    primes_size = 0;
    uint32_t idx = 0;   /* current prime index (starting from 1) */

    /* linear sieve for spf and prime list */
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (new_spf[i] == 0) {                  /* i is prime */
            new_spf[i] = (uint32_t)i;
            ++idx;
            new_prime_index[i] = idx;

            if (primes_size == primes_capacity) {
                primes_capacity <<= 1;
                primes = (uint32_t*)realloc(primes, primes_capacity * sizeof(uint32_t));
            }
            primes[primes_size++] = (uint32_t)i;
        }

        for (size_t j = 0; j < primes_size &&
             primes[j] <= new_spf[i] &&
             (uint64_t)i * primes[j] <= (uint64_t)limit; ++j) {
            uint64_t mult = (uint64_t)i * primes[j];
            new_spf[(size_t)mult] = primes[j];
        }
    }

    /* free old tables */
    free(spf);
    free(prime_index);
    spf = new_spf;
    prime_index = new_prime_index;
    prime_limit = limit;
}

/* ------------------------------------------------------------ */
/* Test whether the integer m satisfies the index–ratio condition. */
static bool factor_and_check(idx_t m)
{
    /* indices of prime factors (with multiplicity) */
    uint32_t idx_arr[32];
    size_t idx_len = 0;

    while (m > 1) {
        uint32_t p = spf[(size_t)m];
        if (p == 0) p = (uint32_t)m;               /* safety catch */
        uint32_t ip = prime_index[p];

        while (m % p == 0) {
            m /= p;
            if (idx_len < 32)
                idx_arr[idx_len++] = ip;
        }
    }

    if (idx_len == 0)         /* m == 1 */
        return true;

    /* sort indices ascending (any order works for the test) */
    for (size_t a = 0; a < idx_len; ++a)
        for (size_t b = a + 1; b < idx_len; ++b)
            if (idx_arr[b] < idx_arr[a]) {
                uint32_t tmp = idx_arr[a];
                idx_arr[a] = idx_arr[b];
                idx_arr[b] = tmp;
            }

    /* check consecutive indices satisfy j <= 2 * i */
    for (size_t a = 1; a < idx_len; ++a) {
        if ((uint64_t)idx_arr[a] > 2ULL * (uint64_t)idx_arr[a - 1])
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* The required sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        terms_capacity = 128;
        terms = (seq_t*)malloc(terms_capacity * sizeof(seq_t));
        terms_size = 0;
        next_scan = 1;
        initialized = true;
    }

    /* extend the sequence until we have n terms */
    while (terms_size < n) {
        /* ensure we have prime data for the number to test */
        if (next_scan > prime_limit) {
            idx_t new_limit = prime_limit * 2;
            if (new_limit < next_scan) new_limit = next_scan;
            init_primes(new_limit);
        }

        if (factor_and_check(next_scan)) {
            if (terms_size == terms_capacity) {
                terms_capacity <<= 1;
                terms = (seq_t*)realloc(terms, terms_capacity * sizeof(seq_t));
            }
            terms[terms_size++] = (seq_t)next_scan;
        }
        ++next_scan;
    }

    return terms[n - 1];
}
