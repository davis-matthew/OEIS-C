/*
 * A361485  Primes p such that p + 1024 is also prime.
 * https://oeis.org/A361485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 882, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that p + 1024 is also prime.  Returns
   the n-th such prime for n >= 1.  The routine uses a simple
   incremental sieve of Eratosthenes to generate primes as needed. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* Internal state ----------------------------------------------------------- */
static uint64_t current_limit = 0;          /* highest number sieved so far   */
static uint8_t *prime_flags = NULL;         /* 0 = assumed prime, 1 = composite */
static size_t flags_cap = 0;                /* allocated bytes for prime_flags */
static uint64_t *qualifiers = NULL;         /* list of primes with p+1024 also prime */
static size_t qual_cap = 0;                 /* allocated size of qualifiers   */
static size_t qual_len = 0;                 /* number of qualifiers found     */
static uint64_t last_checked_limit = 0;     /* limit up to which qualifiers are stable */

/* Resize or allocate the prime flag array up to new_limit (inclusive).   */
static void resize_and_sieve(uint64_t new_limit)
{
    /* Allocate flags if needed. */
    if (flags_cap < new_limit + 1) {
        uint8_t *tmp = (uint8_t *)realloc(prime_flags, new_limit + 1);
        if (!tmp) exit(EXIT_FAILURE);
        prime_flags = tmp;
        flags_cap = new_limit + 1;
    }

    /* Mark all as prime initially (0).  We will leave position 0 and 1 as composite. */
    memset(prime_flags, 0, flags_cap);
    if (new_limit >= 0) prime_flags[0] = 1;
    if (new_limit >= 1) prime_flags[1] = 1;

    /* Eratosthenes sieve. */
    uint64_t i, j;
    for (i = 2; i * i <= new_limit; ++i) {
        if (!prime_flags[i]) {
            for (j = i * i; j <= new_limit; j += i) {
                prime_flags[j] = 1;
            }
        }
    }

    current_limit = new_limit;
}

/* Recompute the qualifiers list up to current_limit. */
static void recompute_qualifiers(void)
{
    /* Discard previous list. */
    free(qualifiers);
    qualifiers = NULL;
    qual_cap = 0;
    qual_len = 0;

    /* Resize to a reasonable initial capacity. */
    qual_cap = 128;
    qualifiers = (uint64_t *)malloc(qual_cap * sizeof(uint64_t));
    if (!qualifiers) exit(EXIT_FAILURE);

    /* Iterate over all p such that p + 1024 <= current_limit. */
    uint64_t p;
    for (p = 2; p + 1024 <= current_limit; ++p) {
        if (!prime_flags[p] && !prime_flags[p + 1024]) {
            /* Add to list. */
            if (qual_len == qual_cap) {
                qual_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(qualifiers, qual_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                qualifiers = tmp;
            }
            qualifiers[qual_len++] = p;
        }
    }
    last_checked_limit = current_limit;
}

/* Public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined; safe default. */

    /* Ensure we have enough qualifiers.  Grow the sieve if necessary. */
    while (qual_len < (size_t)n) {
        /* Grow limit: if no sieve yet, start at 2048; otherwise double. */
        uint64_t new_limit = (current_limit == 0) ? 2048ULL : current_limit * 2ULL;
        /* Perform sieve and recompute qualifiers. */
        resize_and_sieve(new_limit);
        recompute_qualifiers();
        /* If after this expansion we still don't have enough, loop again. */
    }

    /* Return the (n-1)-th zero‑based element, cast to seq_t. */
    return (seq_t)qualifiers[n - 1];
}
