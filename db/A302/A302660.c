/*
 * A302660  a(n) = (prime(n) mod 9) + (prime(n) mod 10).
 * https://oeis.org/A302660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) mod 9) + (prime(n) mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);   /* nth prime */
    uint32_t mod9  = p % 9u;      /* prime(n) mod 9  */
    uint32_t mod10 = p % 10u;     /* prime(n) mod 10 */
    return (seq_t)(mod9 + mod10);
}
