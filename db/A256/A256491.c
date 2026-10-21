/*
 * A256491  a(n) = prime(prime(n) + n - 2).
 * https://oeis.org/A256491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n) + n - 2)
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset 1 */
    uint32_t pn = get_prime(n);          /* the n-th prime */
    idx_t t = (idx_t)pn + n - 2;         /* index for the desired prime */
    return (seq_t)get_prime(t);          /* the t-th prime */
}
