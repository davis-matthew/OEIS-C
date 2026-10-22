/*
 * A339809  a(n) = A019565(n) - 1.
 * https://oeis.org/A339809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A019565(n) - 1
 * Range:     u8 n <= 22, u16 n <= 118, u32 n <= 1022, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A019565(n) = ∏_{i: i-th bit of n is 1} p_{i+1}   (p_k = k-th prime) */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t product = (seq_t)1;
    idx_t m = n;
    idx_t bit = 0U;

    /* iterate over all bits of n */
    while (m > 0U)
    {
        if (m & 1U)
            product = product * (seq_t)get_prime(bit + 1U);
        m >>= 1U;
        ++bit;
    }

    /* subtract 1 – safe because product ≥ 1 */
    return product - (seq_t)1U;
}
