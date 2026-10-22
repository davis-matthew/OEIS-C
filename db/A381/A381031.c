/*
 * A381031  The second smallest prime not dividing n minus the smallest prime
 *          not dividing n.
 * https://oeis.org/A381031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A380539(n) - A053669(n)
 * Range:     u8 n <= 9831, u16 n <= 9831, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the second smallest prime not dividing n minus the smallest prime not dividing n. */

#include <stdbool.h>
#include <stddef.h>

/* Helper from utils */
#include "../utils/build_primes_upto.h"

static bool primes_ready = false;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Build a prime list up to a fixed limit once. The limit is chosen large enough
   to comfortably cover primes needed for any n in the typical range. */
static void
ensure_primes(void)
{
    if (primes_ready)
        return;
    const idx_t LIMIT = 200000;                /* sufficiently large for all test cases */
    build_primes_upto(LIMIT, &primes, &prime_count);
    primes_ready = true;
}

seq_t a(idx_t n)
{
    ensure_primes();

    uint64_t n64 = (uint64_t)n;
    uint64_t first = 0, second = 0;

    /* Find first smallest prime not dividing n */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (n64 % p != 0) {
            first = p;
            /* Find next prime not dividing n */
            for (size_t j = i + 1; j < prime_count; ++j) {
                uint64_t q = primes[j];
                if (n64 % q != 0) {
                    second = q;
                    /* We have both primes */
                    return (seq_t)(second - first);
                }
            }
            /* If we ran out of primes (unlikely), we could extend the list,
               but for typical n this never happens. */
            return (seq_t)0;
        }
    }
    /* Fallback – should not occur */
    return (seq_t)0;
}
