/*
 * A287353  a(0)=0; for n>0, a(n) = 10*a(n-1) + prime(n).
 * https://oeis.org/A287353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n) = 10*a(n-1) + prime(n) for n > 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
        res = res * 10 + (seq_t)get_prime(i);

    return res;
}
