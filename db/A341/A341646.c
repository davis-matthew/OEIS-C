/*
 * A341646  Numbers with a strictly superior squarefree divisor.
 * https://oeis.org/A341646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the A341646 sequence: a(n) is the n-th integer that
   has a strictly superior squarefree divisor.  The function
   builds a prime table once, then iterates numbers until it
   has found the requested amount of sequence values, memoising
   them for future calls.  The test for a number m uses its
   radical rad(m) (product of distinct prime factors); if
   rad(m) > floor_sqrt(m) then m has a strictly superior
   squarefree divisor.  All arithmetic on values that may
   exceed uint64_t is done in seq_t, which may be an
   arbitrary‑precision class.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

static uint64_t *prime_table = NULL;
static size_t prime_count   = 0;

/* Initialise a static sieve of Eratosthenes up to 1 000 000. */
static void init_primes(void) {
    if (prime_table != NULL) return;            /* already built */

    const uint64_t LIMIT = 1000000ULL;
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    /* Rough size estimate for primes <= LIMIT */
    size_t cap = 80000;                        /* ≈ prime count */
    prime_table = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!prime_table) { free(is_composite); exit(EXIT_FAILURE); }

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count == cap) {          /* grow the table */
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_table,
                                                     cap * sizeof(uint64_t));
                if (!tmp) { free(is_composite); free(prime_table);
                              exit(EXIT_FAILURE); }
                prime_table = tmp;
            }
            prime_table[prime_count++] = i;

            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* Cache for sequence values.  seq_cache[i] = a(i+1).  */
static seq_t *seq_cache   = NULL;
static size_t seq_size    = 0;
static size_t seq_capacity = 0;

/* Expand cache to hold at least 'need' terms. */
static void grow_cache(size_t need) {
    if (need <= seq_capacity) return;
    size_t new_cap = seq_capacity ? seq_capacity : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    seq_cache = tmp;
    seq_capacity = new_cap;
}

/* Test whether an integer m has a strictly superior squarefree divisor.
   The divisor must be > sqrt(m) and squarefree. */
static bool has_superior_sqfree_divisor(idx_t m) {
    /* Compute the radical rad(m) = product of distinct prime factors. */
    idx_t n = m;
    seq_t rad = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_table[i];
        if (p > n / p) break;                /* stop when p*p > n */
        if (n % p == 0) {
            rad = rad * (seq_t)p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) rad = rad * (seq_t)n;          /* remaining prime factor */

    seq_t sqrt_m = floor_sqrt((seq_t)m);
    return rad > sqrt_m;
}

/* Main sequence function.  n >= 1.  The function is memoised
   and works correctly even if called with indices out of order. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* protocol: 0 for out of bounds */

    init_primes();                           /* ensure prime table exists */

    /* Increase cache until it contains at least n terms. */
    static idx_t checked_limit = 1;          /* largest number examined so far */
    while (seq_size < n) {
        ++checked_limit;
        if (has_superior_sqfree_divisor(checked_limit)) {
            grow_cache(seq_size + 1);
            seq_cache[seq_size++] = (seq_t)checked_limit;
        }
    }
    return seq_cache[n - 1];
}
