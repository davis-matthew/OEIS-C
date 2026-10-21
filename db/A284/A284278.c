/*
 * A284278  a(1)=2; for n >= 1, if n+2 is prime then a(2*n+1) = 3*n + 2 and
 *          a(2*n) = n + 2, otherwise all terms are 2.
 * https://oeis.org/A284278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>=2, if n is even a(n)=n/2+2; if n is odd let
 *            k=(n-1)/2; if k+2 is prime a(n)=n+2 else a(n)=2
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    if ((n & 1ULL) == 0ULL)                /* n even: a(n) = n/2 + 2 */
        return (seq_t)(n / 2 + 2ULL);

    /* n odd */
    idx_t k = (n - 1ULL) >> 1ULL;           /* k = (n-1)/2 */
    if (is_prime(k + 2ULL))
        return (seq_t)(n + 2ULL);

    return (seq_t)2;
}
