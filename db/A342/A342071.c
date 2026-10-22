/*
 * A342071  Numbers k such that there are more primes in the interval [3*k+1,
 *          4*k] than there are in the interval [2*k+1, 3*k].
 * https://oeis.org/A342071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A342071: k such that π(4k)−π(3k) > π(3k)−π(2k) */
/* This function builds a prime sieve on demand and caches the terms
   of the sequence in a fixed array.  The implementation works
   for all supported seq_t and idx_t types. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- sieve state --------------------------------------- */
static idx_t prime_limit = 0;            /* maximum number sieved so far   */
static uint64_t *prime_bits = NULL;      /* bitset: 1 ⇒ prime, 0 ⇒ composite */
static uint32_t *pi_prefix  = NULL;      /* prefix prime counts            */

/* set / test bits */
static inline void clear_prime_bit(idx_t n) {
    prime_bits[n >> 6] &= ~(1ULL << (n & 63));
}
static inline bool test_prime_bit(idx_t n) {
    return (prime_bits[n >> 6] >> (n & 63)) & 1U;
}

/* build (or extend) the sieve up to 'limit' (inclusive). */
static void ensure_sieve(idx_t limit) {
    if (limit <= prime_limit)
        return;               /* already sufficient */

    /* free previous allocations */
    if (prime_bits) free(prime_bits);
    if (pi_prefix)  free(pi_prefix);
    prime_bits = NULL;
    pi_prefix  = NULL;

    /* allocate bitset */
    size_t words = (limit + 1 + 63) >> 6;
    prime_bits = (uint64_t*)calloc(words, sizeof(uint64_t));
    if (!prime_bits) abort();

    /* all set to prime initially, then clear 0 and 1 */
    for (size_t w = 0; w < words; ++w)
        prime_bits[w] = ~0ULL;
    clear_prime_bit(0);
    clear_prime_bit(1);

    /* sieve */
    idx_t sqrt_lim = (idx_t)sqrt((double)limit);
    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        if (test_prime_bit(p)) {
            for (idx_t m = p * p; m <= limit; m += p)
                clear_prime_bit(m);
        }
    }

    /* prefix prime counts */
    pi_prefix = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!pi_prefix) abort();
    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi_prefix[i] = pi_prefix[i-1] + (uint32_t)test_prime_bit(i);

    prime_limit = limit;
}

/* ---------- cache for sequence terms ------------------------ */
#define TERM_CAPACITY 2048
static idx_t term_cache[TERM_CAPACITY];
static size_t term_size = 0;
static idx_t last_k = 0;

/* ----------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for n = 0 */

    /* extend cache until we have at least n terms or a guard is hit */
    while (term_size < (size_t)n) {
        ++last_k;

        /* expand sieve as needed */
        ensure_sieve(4 * last_k);

        /* prime counts in the two intervals */
        uint32_t pi4k = pi_prefix[4 * last_k];
        uint32_t pi3k = pi_prefix[3 * last_k];
        uint32_t pi2k = pi_prefix[2 * last_k];

        uint32_t first  = pi4k - pi3k;  /* primes in [3k+1,4k]   */
        uint32_t second = pi3k - pi2k;  /* primes in [2k+1,3k]   */

        if (first > second) {
            if (term_size < TERM_CAPACITY)
                term_cache[term_size++] = last_k;
            else
                break;  /* capacity exceeded – shouldn't happen */
        }

        /* safety guard: stop if we exceed known bounds of the sequence */
        if (last_k > 2000000ULL)   /* far beyond the largest known term */
            break;
    }

    if (term_size >= (size_t)n && n <= TERM_CAPACITY)
        return (seq_t)term_cache[n - 1];
    else
        return 0;   /* no further term found within computed range */
}
