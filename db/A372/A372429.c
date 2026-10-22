/*
 * A372429  Sum of binary indices of prime(n). Sum of positions of ones in the
 *          reversed binary expansion of prime(n).
 * https://oeis.org/A372429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9438, u16 n <= 9438, u32 n <= 9438, u64 n <= 9438,
 *            u128 n <= 9438, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of positions of 1-bits in the nth prime's binary representation
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute the sum of the bit indices (LSB has index 1). */
    seq_t sum  = 0;
    idx_t  pos = 1;
    uint64_t tmp = p;          /* use 64‑bit to shift safely */

    while (tmp)
    {
        if (tmp & 1U)
            sum += (seq_t)pos;
        pos++;
        tmp >>= 1;
    }

    return sum;
}
