/*
 * A323162  a(n) = 1 if both n and n-1 are composite, 0 otherwise.
 * https://oeis.org/A323162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if both n and n-1 are composite, 0 otherwise
 * Range:     u8 n <= 9841, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements using the supplied is_prime function from utils. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;
    if (!is_prime(n) && !is_prime(n-1))
        return (seq_t)1;
    return (seq_t)0;
}
