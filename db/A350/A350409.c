/*
 * A350409  Primes p such that 2*p+1 has exactly three prime factors (not
 *          necessarily distinct).
 * https://oeis.org/A350409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   p is prime and 2*p+1 has exactly three prime factors (counted
 *            with multiplicity)
 * Range:     u8 n <= 7, u16 n <= 1232, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;   /* found primes satisfying the condition */
static size_t cache_size = 0; /* number of primes stored */
static size_t cache_cap = 0;  /* allocated capacity */

static idx_t next_candidate = 2; /* next integer to test for primality */

/* Return the nth element of the sequence (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* value for offset 0 is irrelevant */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        if (is_prime(next_candidate)) {
            /* Check the number of prime factors of 2*p + 1. */
            seq_t mp = (seq_t)2 * (seq_t)next_candidate + 1;
            if (count_prime_factors(mp) == 3) {
                /* Expand cache if necessary. */
                if (cache_cap < cache_size + 1)
                    grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = next_candidate;
            }
        }

        /* Advance to the next candidate: after 2, skip even numbers. */
        if (next_candidate < 2)
            next_candidate = 2;
        else if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
