/*
 * A261198  Start with n and repeat the map x -> x+sumdigits(x) until reaching
 *          a prime, which is a(n), or 0 if no prime is reached.
 * https://oeis.org/A261198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the first prime reached by repeatedly applying * x -> x
 *            + sumdigits(x) to the starting value n (after at least one *
 *            application). If this process never reaches a prime, the
 *            function * returns 0. A multiple of 3 will never reach a prime
 *            other than 3, * so the function stops immediately wh...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but guard anyway. */
    if (n == 0) return 0;

    /* Multiples of 3 can never reach a prime (except the prime 3 itself,
     * which is excluded by the following check). */
    if ((n % 3) == 0) return 0;

    seq_t x = (seq_t)n;          /* a0 – not checked for primality */

    /* Perform the iteration.  A safety limit prevents an infinite loop
     * in pathological cases; 10,000 steps is far more than enough for
     * the first 10 000 terms. */
    for (unsigned int i = 0; i < 10000; ++i) {
        x += digit_sum(x);       /* a_{k+1} = a_k + sumdigits(a_k) */

        if (is_prime_big(x)) return x;   /* prime found */

        if ((x % 3) == 0) return 0;      /* will never reach a prime */
    }

    /* If the safety limit is hit, assume no prime is reachable. */
    return 0;
}
