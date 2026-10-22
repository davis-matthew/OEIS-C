/*
 * A383297  Numbers k for which A276086(A006530(k-1)) divides A276086(k),
 *          where A006530 gives the largest prime factor of k, and A276086 is
 *          the primorial base exp-function.
 * https://oeis.org/A383297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 6450, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A383297: a(n) = the nth integer k (k ≥ 3) such that
    A276086(A006530(k-1)) divides A276086(k).
    In primorial-base notation this is equivalent to
    each digit of the largest prime factor of (k‑1) being
    component‑wise ≤ the corresponding digit of k.  */

/*  A very small static table of the first primes needed for the
    mixed radix “primorial” representation.  The product of the first
    20 primes far exceeds any k that will appear for the first 10 000
    terms of the sequence. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"

static const uint32_t PRIMES[] = {
    2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
    31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U,
    73U, 79U, 83U, 89U, 97U
};
static const size_t NUM_PRIMES = sizeof(PRIMES)/sizeof(PRIMES[0]);

/*  Compute the primorial‑base digits of n into out[NUM_PRIMES].
    For indices beyond the representation length the digits are
    set to 0.  */
static void primorial_digits(uint64_t n, uint32_t out[NUM_PRIMES])
{
    uint64_t tmp = n;
    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        out[i] = (uint32_t)(tmp % PRIMES[i]);
        tmp   /= PRIMES[i];
        if (tmp == 0) {
            for (size_t j = i + 1; j < NUM_PRIMES; ++j)
                out[j] = 0;
            break;
        }
    }
}

/*  Return true iff k satisfies the divisibility condition.  */
static bool satisfies(uint64_t k)
{
    if (k < 3U) return false;                 /* sequence starts at 3 */

    uint64_t p = largest_prime_factor(k - 1); /* A006530(k-1)   */
    if (p == 0) return false;                 /* safety guard    */

    uint32_t dk[NUM_PRIMES];
    uint32_t dp[NUM_PRIMES];
    primorial_digits(k, dk);
    primorial_digits(p, dp);

    for (size_t i = 0; i < NUM_PRIMES; ++i)
        if (dp[i] > dk[i]) return false;
    return true;
}

/*  Return the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined: harmless for n≥1 */

    /*  Cached list of sequence values.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;

    /*  Next integer to test for the property.  */
    static uint64_t next_candidate = 3U;

    if (cache == NULL) {
        cache_capacity = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        while (!satisfies(next_candidate))
            ++next_candidate;

        /* Resize storage if needed. */
        if (cache_size >= cache_capacity) {
            cache_capacity *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_capacity);
        }
        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
