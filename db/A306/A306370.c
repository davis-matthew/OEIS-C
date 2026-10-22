/*
 * A306370  Primes p such that none of p+6, p+12, p+18, p+30 is prime.
 * https://oeis.org/A306370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the n‑th prime p for which none of p+6, p+12, p+18, p+30
   is prime (offset 1). The routine builds a sieve up to a limit
   large enough to contain at least the first 10 000 such primes,
   collects the qualified primes into a static array, and returns
   the requested element.  */

/* --------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following constants can be tuned if more terms are required. */
static const size_t SIEVE_LIMIT = 20000000U;   /* 20 million – ample for 10 000 terms */
static const size_t INITIAL_QCAP = 20000U;     /* capacity for collected primes */

/* static state */
static bool initialized = false;
static uint8_t *is_composite = NULL; /* 0 = prime, 1 = composite */
static seq_t *qual_primes    = NULL; /* primes satisfying the condition */
static size_t qual_count     = 0;

/* --------------------------------------------------------------- */
static void init_sieve(void)
{
    /* Allocate composite flag array */
    is_composite = (uint8_t*)calloc(SIEVE_LIMIT + 1U, sizeof(uint8_t));
    if (!is_composite) exit(EXIT_FAILURE);
    is_composite[0] = 1U;
    is_composite[1] = 1U;

    /* Sieve of Eratosthenes */
    for (uint32_t i = 2U; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            uint64_t start = (uint64_t)i * i;
            for (uint64_t j = start; j <= SIEVE_LIMIT; j += i)
                is_composite[(size_t)j] = 1U;
        }
    }

    /* Allocate array for qualifying primes */
    qual_primes = (seq_t*)malloc(INITIAL_QCAP * sizeof(seq_t));
    if (!qual_primes) exit(EXIT_FAILURE);
    qual_count = 0;
    size_t qcap   = INITIAL_QCAP;

    /* Scan all primes and keep those meeting the condition */
    for (size_t p = 2; p + 30 <= SIEVE_LIMIT; ++p) {
        if (!is_composite[p]) {
            /* All of p+6, p+12, p+18, p+30 must be composite */
            if (is_composite[p + 6] &&
                is_composite[p + 12] &&
                is_composite[p + 18] &&
                is_composite[p + 30]) {

                if (qual_count == qcap) {            /* grow if needed */
                    qcap *= 2U;
                    seq_t *tmp = (seq_t*)realloc(qual_primes,
                                                qcap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    qual_primes = tmp;
                }
                qual_primes[qual_count++] = (seq_t)p;
            }
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sieve();

    if (n >= 1U && (size_t)n <= qual_count)
        return qual_primes[(size_t)n - 1U];

    /* out of range – 0 indicates "not found" */
    return (seq_t)0;
}
