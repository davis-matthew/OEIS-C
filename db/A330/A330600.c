/*
 * A330600  Primes p such that the sum of (p mod d) for nonzero digits d of p
 *          is prime.
 * https://oeis.org/A330600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime p such that the sum of (p mod d) over all
 *            non‑zero decimal digits d of p is also prime. The sequence is
 *            generated on demand and memoised
 * Range:     u8 n <= 26, u16 n <= 2492, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cached = NULL;
    static size_t cached_len = 0;   /* number of terms stored */
    static size_t cached_cap = 0;   /* allocated capacity */
    /* static next candidate to test for primality */
    static seq_t next_candidate = 2;

    /* initialise cache */
    if (cached_cap == 0) {
        /* initial capacity 16 */
        ensure_capacity(16, &cached_cap, (void **)&cached, sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cached_len < (size_t)n) {
        /* find next prime candidate */
        seq_t p;
        for (p = next_candidate; ; ++p) {
            if (isPrimeGeneric(p))
                break;
        }

        /* compute sum of (p mod d) for each non‑zero digit d of p */
        seq_t sum = 0;
        seq_t tmp = p;
        while (tmp > (seq_t)0) {
            seq_t d = tmp % (seq_t)10;
            if (d != (seq_t)0)
                sum += p % d;
            tmp /= (seq_t)10;
        }

        /* if the sum is prime, store p */
        if (isPrimeGeneric(sum)) {
            ensure_capacity(cached_len + 1, &cached_cap, (void **)&cached, sizeof(seq_t));
            cached[cached_len++] = p;
        }

        /* advance candidate for next iteration */
        next_candidate = p + (seq_t)1;
    }

    return cached[n - 1];
}
