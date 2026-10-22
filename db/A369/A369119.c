/*
 * A369119  a(n) = (n + 1)^[is_prime(n + 1)] * n!.
 * https://oeis.org/A369119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n+1)^[is_prime(n+1)]
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t res = 1;

    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;

    if (is_prime(n + 1))
        res *= (seq_t)(n + 1);

    return res;
}
