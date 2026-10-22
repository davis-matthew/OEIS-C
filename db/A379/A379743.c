/*
 * A379743  a(n) is the smallest prime whose digital sum in base n is n.
 * https://oeis.org/A379743
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest prime p such that the base‑n digit sum of p
 *            equals n
 * Range:     u8 n <= 31, u16 n <= 1691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iterates over all primes p >= 2 (using a simple primality test)
   and tests whether the sum of the digits of p written in base n equals n.  The first
   prime that satisfies this condition is returned.  The implementation uses only
   integer arithmetic and the utilities supplied by the framework. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Test whether a 64‑bit integer is prime.  The helper uses a fast deterministic
   Miller‑Rabin test for 64‑bit numbers.  (It is provided by the standard
   utilities.) */
#include "../utils/is_prime.h"

/* Test whether an arbitrary-precision seq_t number is prime.  The helper
   accepts a seq_t value and uses a suitable primality test. */
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum_base.h"

/*----------------------------------------------------------------------------------------------------------------------------------
 * Helper: test whether a seq_t number is prime (handles both 64‑bit and larger).
 *----------------------------------------------------------------------------------------------------------------------------------*/
static inline bool is_prime_seq(seq_t n)
{
    if (n <= 1)
        return false;
    if (n <= ULLONG_MAX)
        return is_prime((idx_t)n);
    return is_prime_big(n);
}

/*----------------------------------------------------------------------------------------------------------------------------------
 * Main sequence function.
 *----------------------------------------------------------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)               /* offset of the sequence is 2; values before that are irrelevant */
        return 0;

    /* start testing from the first prime, 2 */
    seq_t p = 2;
    while (true)
    {
        if (is_prime_seq(p) &&
            digit_sum_base(p, n) == (seq_t)n)
            return p;

        /* next candidate prime */
        if (p == 2)
            p = 3;               /* after 2, only odd primes exist */
        else
            p += 2;               /* skip even numbers */
    }
}
