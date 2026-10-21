/*
 * A283118  a(n) = sigma(5*n).
 * https://oeis.org/A283118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(5*n)
 * Range:     u8 n <= 23, u16 n <= 3527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Return the sum of divisors of 5*n. */
    return sigma_of((idx_t)5 * n);
}
