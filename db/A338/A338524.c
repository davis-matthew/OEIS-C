/*
 * A338524  prime(n) Gray code decoding.
 * https://oeis.org/A338524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006068(A000040(n))
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime, gray_decode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/gray_decode.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑based).  get_prime returns a uint32_t. */
    seq_t prime = (seq_t)get_prime(n);
    return gray_decode(prime);
}
