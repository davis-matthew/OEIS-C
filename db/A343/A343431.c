/*
 * A343431  Part of n composed of prime factors of the form 6k-1.
 * https://oeis.org/A343431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^e for all prime powers p^e dividing n with p
 *            ≡ 5 (mod 6)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 1;        /* a(1) = 1, and any n < 1 is treated as 1 */

    seq_t result = 1;
    idx_t t = n;                /* work on a copy of n */

    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            idx_t exponent = 0;
            do {
                t /= p;
                ++exponent;
            } while (t % p == 0);

            if (p % 6 == 5) {                     /* prime of the form 6k‑1 */
                for (idx_t i = 0; i < exponent; ++i)
                    result *= (seq_t)p;
            }
        }
    }

    /* t now is 1 or a prime factor > sqrt(original n) */
    if (t > 1 && t % 6 == 5)
        result *= (seq_t)t;

    return result;
}
