/*
 * A360790  Squared length of diagonal of right trapezoid with three
 *          consecutive prime length sides.
 * https://oeis.org/A360790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 + (prime(n+2)-prime(n+1))^2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the required primes */
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);
    uint32_t p_np2 = get_prime(n + 2);

    /* compute the two squares */
    seq_t sq1 = (seq_t)p_n * (seq_t)p_n;
    uint32_t diff = p_np2 - p_np1;
    seq_t sq2 = (seq_t)diff * (seq_t)diff;

    /* return the sum */
    return sq1 + sq2;
}
