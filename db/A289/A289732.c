/*
 * A289732  Primes of the form k!6-18, where k!6 is the sextuple factorial
 *          number (A085158).
 * https://oeis.org/A289732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime numbers p such that p + 18 = k!6 where k!6 =
 *            product of all integers ≤ k that are congruent to k modulo 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Product of numbers ≡ r (mod 6) seen so far, for each r = 0 … 5. */
    seq_t prod[6] = {1, 1, 1, 1, 1, 1};

    seq_t candidate;
    size_t seen = 0;
    idx_t k = 0;

    while (seen < n) {
        ++k;
        int r = k % 6;
        seq_t tmp = prod[r] * k;

        /* Detect integer overflow (if seq_t is an unsigned built‑in type). */
        if (tmp < prod[r])
            break;                    /* no further values fit in seq_t */

        prod[r] = tmp;

        if (tmp > (seq_t)18) {        /* avoid underflow when subtracting 18 */
            candidate = tmp - (seq_t)18;
            if (is_prime_big(candidate)) {
                ++seen;
                if (seen == n)
                    return candidate;
            }
        }
    }
    return (seq_t)0;                    /* should not happen for valid inputs */
}
