/*
 * A271211  Composite integers sandwiched between primes p, q with q-p = 4.
 * https://oeis.org/A271211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function generates the sequence A271211. A number m is in
 *            the sequence iff there exist consecutive primes p < m < q such
 *            that q-p = 4. For each such pair, all composite integers between
 *            p and q are added to the sequence. The first composite that
 *            appears is 8 (between 7 and 11)
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* static data -------------------------------------------------------- */

static bool initialized = false;

/* sieve data */
#define SIEVE_LIMIT 12000000   /* 12 million is enough for n<=10000 */
static uint8_t *is_prime_arr = NULL;
static const size_t MAX_PRIMES = 1000000;
static uint32_t *primes = NULL;
static size_t prime_count = 0;

/* sequence data */
static seq_t *seq_terms = NULL;
static size_t seq_len = 0;

/*--------------------------------------------------------------------*/

static void init_sequence(void)
{
    if (initialized)
        return;

    /* 1. sieve of Eratosthenes */
    size_t lim = SIEVE_LIMIT;
    is_prime_arr = (uint8_t *)malloc(lim + 1);
    for (size_t i = 0; i <= lim; ++i)
        is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;

    size_t r = (size_t)sqrt((double)lim);
    for (size_t i = 2; i <= r; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                is_prime_arr[j] = 0;
        }
    }

    /* 2. collect primes into table */
    primes = (uint32_t *)malloc(MAX_PRIMES * sizeof(uint32_t));
    prime_count = 0;
    for (size_t i = 2; i <= lim && prime_count < MAX_PRIMES; ++i) {
        if (is_prime_arr[i]) {
            primes[prime_count++] = (uint32_t)i;
        }
    }

    /* 3. build the sequence from pairs of consecutive primes
       with difference 4 and prime >= 7.  There are 3 numbers per pair. */
    size_t est_terms = prime_count / 5 * 3 + 10; /* rough estimate */
    seq_terms = (seq_t *)malloc(est_terms * sizeof(seq_t));
    seq_len = 0;

    for (size_t i = 0; i + 1 < prime_count; ++i) {
        uint32_t p = primes[i];
        uint32_t q = primes[i + 1];
        if (q - p == 4 && p >= 7) {
            seq_terms[seq_len++] = (seq_t)(p + 1);
            seq_terms[seq_len++] = (seq_t)(p + 2);
            seq_terms[seq_len++] = (seq_t)(p + 3);
        }
    }

    /* shrink to actual size */
    seq_terms = (seq_t *)realloc(seq_terms, seq_len * sizeof(seq_t));

    initialized = true;
}

/*--------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    init_sequence();

    /* n is 1‑based.  The caller guarantees n>=1. */
    return seq_terms[n - 1];
}
