/*
 * A379371  Primes congruent to 67 mod 12345.
 * https://oeis.org/A379371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 67 (mod 24690) */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t candidate = 67;          /* first number in the progression */
    const seq_t step    = 24690;   /* common difference of the arithmetic progression */
    idx_t count = 0;

    while (1)
    {
        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += step;
    }
}
