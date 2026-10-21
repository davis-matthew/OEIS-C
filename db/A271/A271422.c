/*
 * A271422  Concatenation of prime(n) and its square.
 * https://oeis.org/A271422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Concatenate[prime(n), prime(n)^2]
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   concat_parts, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime number */
    uint32_t p = get_prime(n);

    /* Compute its square, cast to idx_t to avoid overflow in 32‑bit arithmetic */
    idx_t sq = (idx_t)p * (idx_t)p;

    /* Concatenate the prime and its square */
    return concat_parts((idx_t)p, sq);
}
