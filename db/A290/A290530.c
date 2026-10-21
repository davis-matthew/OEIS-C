/*
 * A290530  Primes congruent to {11, 17} mod 30.
 * https://oeis.org/A290530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 11 or 17 modulo 30
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(1) = 11, a(2) = 17, a(3) = 41, ... (offset 1) */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The nth prime we are interested in. */
    idx_t count = 0U;
    idx_t candidate = 10U;    /* start just before the first prime 11 */

    for (;;) {
        /* Skip even numbers (except 2, which never occurs here). */
        do { ++candidate; } while ((candidate & 1U) == 0U);

        /* Keep only numbers ≡ 11 or 17 (mod 30). */
        if (candidate % 30U == 11U || candidate % 30U == 17U) {
            if (is_prime(candidate)) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
    }
}
