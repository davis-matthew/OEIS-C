/*
 * A386606  Numbers with exactly 3 distinct odd prime factors.
 * https://oeis.org/A386606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386606: numbers with exactly 3 distinct odd prime factors.
   The implementation enumerates natural numbers, checks the
   distinct odd prime factor count and caches the results. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* internal state ------------------------------------------------------- */
static bool initialized = false;

/* prime list */
static uint32_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* candidate number to test next (starts at 3) */
static idx_t current_candidate = 3;

/* maximum number to sieve (chosen to cover at least the first 10000 terms) */
#define SIEVE_LIMIT 2000000

/* --------------------------------------------------------------------- */

/* Ensure the prime list and isComposite sieve are built. */
static void init(void)
{
    if (initialized) return;

    /* allocate isComposite array and sieve */
    bool *is_composite = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(1); /* allocation failed */

    /* mark evens as composite except 2 */
    for (idx_t i = 4; i <= SIEVE_LIMIT; i += 2) is_composite[i] = true;

    for (idx_t i = 3; i * i <= SIEVE_LIMIT; i += 2) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += 2 * i)
                is_composite[j] = true;
        }
    }

    /* collect primes into dynamic array */
    primes_cap = 200000;          /* initial capacity */
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_len == primes_cap) { /* grow */
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_len++] = (uint32_t)i;
        }
    }

    free(is_composite);
    initialized = true;
}

/* Ensure the dynamic array has enough space for one more element. */
static void ensure_cache_capacity(void)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 4;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }
}

/* Count the distinct odd prime factors of n. */
static int count_distinct_odd_primes(idx_t n)
{
    if (n <= 1) return 0;

    /* remove powers of 2 (not counted) */
    while ((n & 1ULL) == 0) n >>= 1;

    int count = 0;
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > n) break;
        if (p == 2) continue;            /* skip even prime */
        if (n % p == 0) {
            ++count;
            if (count > 3) return count;
            do { n /= p; } while (n % p == 0);
        }
    }
    if (n > 1) {                         /* leftover prime factor */
        if (n > 2 && n % 2 == 1) {
            ++count;
        }
    }
    return count;
}

/* --------------------------------------------------------------------- */
/* Main function ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    init();

    while (seq_len < n) {
        if (count_distinct_odd_primes(current_candidate) == 3) {
            ensure_cache_capacity();
            seq_cache[seq_len++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }
    return seq_cache[n - 1];
}
