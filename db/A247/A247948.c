/*
 * A247948  Five-digit odd semiprimes with all digits distinct.
 * https://oeis.org/A247948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3175, u32 n <= 4858, u64 n <= 4858,
 *            u128 n <= 4858, big any n
 * Helpers:   append_candidate, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the n-th five‑digit odd semiprime with all digits distinct.
    This is done by generating the complete list once and returning the
    requested element.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/build_primes_upto.h"

/* Test whether all decimal digits of a 5‑digit integer are different. */
static inline bool distinct_digits(uint64_t v)
{
    bool used[10] = { false };
    while (v) {
        int d = v % 10;
        if (used[d])
            return false;
        used[d] = true;
        v /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static storage for the sequence. */
    static seq_t *seq = NULL;   /* array of values                      */
    static size_t sz  = 0;      /* current number of stored values      */
    static size_t cap = 0;      /* capacity of the allocated array      */

    /* Build the sequence once.  */
    if (seq == NULL) {
        /* Small primes up to sqrt(99999) = 316. */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto((idx_t)316, &primes, &prime_cnt);

        /* Scan all five‑digit numbers. */
        for (uint64_t x = 10000; x <= 99999; ++x) {
            if ((x & 1u) == 0)                       /* skip even numbers */
                continue;
            if (!distinct_digits(x))
                continue;

            /* Find the smallest prime factor of x. */
            uint64_t spf = 0;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > x)
                    break;
                if (x % p == 0) {
                    spf = p;
                    break;
                }
            }
            if (spf == 0)               /* x is prime, not semiprime */
                continue;

            uint64_t q = x / spf;
            if (!is_prime_big((seq_t)q))
                continue;               /* not semiprime */

            /* Store the value. */
            append_candidate(&seq, &sz, &cap, (seq_t)x);
        }

        /* The primes array was allocated by build_primes_upto; free it. */
        free(primes);
    }

    if (n < 1)
        return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= sz)
        return 0;                      /* out of bounds (should not happen) */
    return seq[idx];
}
