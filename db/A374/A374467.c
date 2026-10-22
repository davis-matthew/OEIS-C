/*
 * A374467  Numbers that are products of an odd number of primes and the total
 *          number of 1-bits in the exponents of their prime factorization n
 *          is odd.
 * https://oeis.org/A374467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 8784, u32 n <= 8784, u64 n <= 8784,
 *            u128 n <= 8784, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search through integers, testing each for
   odd number of prime factors counted with multiplicity (Ω(n) odd)
   and odd total number of 1‑bits in the exponents of the prime factorization. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether a 64‑bit integer has odd Ω(n) and odd popcount of its exponents. */
static bool is_special(uint64_t x)
{
    unsigned int omega = 0;          /* total number of prime factors (with multiplicity) */
    unsigned int bitsum = 0;         /* total popcount of the exponents */

    uint64_t t = x;

    /* factor the power of 2 */
    if ((t & 1ULL) == 0) {
        unsigned int exp = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            ++exp;
            ++omega;
        }
        bitsum += __builtin_popcount(exp);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned int exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
                ++omega;
            }
            bitsum += __builtin_popcount(exp);
        }
    }

    /* remaining prime factor (if any) */
    if (t > 1) {
        ++omega;
        bitsum += 1;   /* exponent 1 has popcount 1 */
    }

    return ((omega & 1U) && (bitsum & 1U));
}

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static uint64_t next_candidate = 2; /* next integer to test */

    while (cache_len < (size_t)n) {
        if (is_special(next_candidate)) {
            seq_t *tmp = (seq_t*)realloc(cache,
                                         (cache_len + 1) * sizeof(seq_t));
            if (!tmp) abort();           /* memory allocation failed */
            cache = tmp;
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
