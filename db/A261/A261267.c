/*
 * A261267  Primes having only {0, 2, 7} as digits.
 * https://oeis.org/A261267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 1644, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime consisting only of the digits 0, 2 and 7
   (OEIS sequence A261267).  The implementation enumerates all numbers
   whose decimal representation uses only these digits, tests them for
   primality using the supplied isPrimeGeneric helper, and caches the
   results so that subsequent calls are O(1).  The generation order is
   strictly increasing in numeric value, matching the OEIS order. */
#include <stdlib.h>
#include <stdbool.h>

/* Primality test helper supplied by the build environment. */
#include "../utils/isPrimeGeneric.h"

/* Cache of discovered primes. */
static seq_t *primes = NULL;          /* array of primes */
static size_t primes_cap = 0;         /* allocated capacity */
static size_t primes_len = 0;         /* number of primes stored */

/* Append a prime to the cache, expanding the buffer if necessary. */
static void add_prime(seq_t val)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = val;
}

/* Recursively generate numbers of a fixed length that satisfy the
   digit restriction (first digit 2 or 7, last digit 7 for length ≥ 2,
   middle digits 0, 2, 7).  Numbers are produced in ascending numeric
   order. */
static const unsigned mid_digits[3] = {0, 2, 7};

static void rec_generate(int len, int pos, seq_t curr)
{
    if (pos == len) {
        if (isPrimeGeneric(curr))
            add_prime(curr);
        return;
    }

    if (pos == 0) {
        /* First digit: 2 or 7 */
        for (unsigned d = 2; d <= 7; d += 5) {
            seq_t nx = curr * 10 + d;
            if (nx < curr)   /* overflow guard for fixed-width types */
                continue;
            rec_generate(len, pos + 1, nx);
        }
    } else if (pos == len - 1) {
        /* Last digit (for length ≥ 2): must be 7 */
        seq_t nx = curr * 10 + 7;
        if (nx < curr)
            return;
        rec_generate(len, pos + 1, nx);
    } else {
        /* Middle digits: 0, 2, or 7 */
        for (int i = 0; i < 3; ++i) {
            unsigned d = mid_digits[i];
            seq_t nx = curr * 10 + d;
            if (nx < curr)
                continue;
            rec_generate(len, pos + 1, nx);
        }
    }
}

/* Generate all numbers of a specific length and add the primes among them. */
static void gen_len(int len)
{
    if (len == 1) {
        /* Length 1: 2 and 7 */
        if (isPrimeGeneric((seq_t)2))
            add_prime((seq_t)2);
        if (isPrimeGeneric((seq_t)7))
            add_prime((seq_t)7);
    } else {
        rec_generate(len, 0, (seq_t)0);
    }
}

/* Public accessor for the n‑th term of the sequence.
   The first term has index 1 (a(1) = 2).  Calls may happen in any order;
   the result is always returned from the cache after generating enough
   entries. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    static int next_len = 1; /* next number length to generate */

    /* Generate lengths until we have enough primes. */
    while (primes_len < (size_t)n) {
        gen_len(next_len++);
    }
    return primes[n - 1];
}
