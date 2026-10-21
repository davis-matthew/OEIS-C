/*
 * A272550  Lexicographically earliest increasing sequence of primes such that
 *          odd-indexed terms have final digit 1 and even-indexed terms have
 *          final digit 9.
 * https://oeis.org/A272550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: greedy search for primes ending in 1/9 alternately.
   a(1) = 11, a(2) = 19, a(3) = 31, …  a(n) is the nth prime found
   that satisfies the alternating digit condition. */

#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"

static seq_t *terms = NULL;      /* cached terms */
static size_t terms_len = 0;     /* number of terms already generated */
static size_t terms_cap = 0;     /* current capacity of the cache */

/* Ensure the cache can hold at least 'needed' elements. */
static void
ensure_cache_size(size_t needed)
{
    if (needed <= terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_terms)
        abort(); /* memory allocation failed */

    if (terms) {
        memcpy(new_terms, terms, terms_len * sizeof(seq_t));
        free(terms);
    }

    terms     = new_terms;
    terms_cap = new_cap;
}

/* Return the first prime ≥ start whose last decimal digit is d (1 or 9). */
static uint64_t
next_prime_with_digit(uint64_t start, uint8_t d)
{
    uint64_t p = next_prime(start);
    while (p % 10 != d)
        p = next_prime(p + 1);
    return p;
}

/* External linkage as required. */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for index 0, but not used */

    size_t needed = (size_t)n;    /* n is expected to fit in size_t */
    if (terms_len < needed) {
        ensure_cache_size(needed);

        uint64_t last = 0;
        if (terms_len > 0)
            last = (uint64_t)terms[terms_len - 1];

        for (size_t k = terms_len; k < needed; ++k) {
            /* k is 0‑based: even k -> a(n) with odd n -> last digit 1
               odd  k -> a(n) with even n -> last digit 9. */
            uint8_t digit = (k % 2 == 0) ? 1 : 9;
            uint64_t next_p = next_prime_with_digit(last + 1, digit);
            terms[k] = (seq_t)next_p;
            last = next_p;
        }
        terms_len = needed;
    }

    return terms[n - 1];
}
