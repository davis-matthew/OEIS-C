/*
 * A320687  Sum of differences of the larger square and primes between two
 *          squares.
 * https://oeis.org/A320687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime in (n^2,(n+1)^2)} ((n+1)^2 – p)
 * Range:     u8 n <= 13, u16 n <= 610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small primes up to 10001 (√(10001²)) */
#define SMALL_PRIME_LIMIT 10001
static uint32_t small_primes[1229]; /* enough for primes <= 10001 */
static size_t small_prime_count = 0;

/* One-time initialization of the small primes array */
static void init_small_primes(void) {
    if (small_prime_count != 0) return;
    bool is_composite[SMALL_PRIME_LIMIT + 1] = {0};
    is_composite[0] = is_composite[1] = true;
    for (uint32_t i = 2; i * i <= SMALL_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                is_composite[j] = true;
        }
    }
    size_t idx = 0;
    for (uint32_t i = 2; i <= SMALL_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) small_primes[idx++] = i;
    }
    small_prime_count = idx;
}

/* a(n) as defined above */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, undefined for n=0 */

    init_small_primes();

    /* Compute n^2 and (n+1)^2 */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t n1_sq  = (seq_t)(n + 1) * (seq_t)(n + 1);
    seq_t low    = n_sq + 1;          /* first number in interval */
    seq_t high   = n1_sq - 1;         /* last number in interval */
    size_t range_len = (size_t)(high - low + 1); /* equals 2*n */

    /* Segmented sieve for the current interval */
    uint8_t *segment = (uint8_t *)malloc(range_len);
    if (!segment) return 0; /* memory failure – not expected for small ranges */
    for (size_t i = 0; i < range_len; ++i) segment[i] = 1; /* assume prime */

    /* Mark composites in the segment using small primes */
    for (size_t s = 0; s < small_prime_count; ++s) {
        uint32_t p = small_primes[s];
        seq_t p_sq = (seq_t)p * (seq_t)p;
        if (p_sq > high) break;            /* primes beyond sqrt(high) not needed */

        /* Find first multiple of p in [low, high] */
        seq_t start = (low + p - 1) / p * p; /* ceil(low/p)*p */
        if (start < p) start = p * 2;        /* start from 2*p if below p*2 */
        else if (start == p) start += p;     /* skip marking p itself */

        for (seq_t j = start; j <= high; j += p) {
            segment[(size_t)(j - low)] = 0; /* composite */
        }
    }

    /* Sum differences for remaining primes */
    seq_t sum_diff = 0;
    for (size_t i = 0; i < range_len; ++i) {
        if (segment[i]) {
            seq_t prime = low + (seq_t)i;
            sum_diff += n1_sq - prime;
        }
    }

    free(segment);
    return sum_diff;
}
