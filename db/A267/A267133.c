/*
 * A267133  a(n) = (1/n)(2/n)(3/n)...((n-1)/n) where (k/n) is the Kronecker
 *          symbol, n >= 1.
 * https://oeis.org/A267133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n = 1 or 2; 0 if n is composite; for odd prime n,
 *            a(n) = (-1)^((n-1)/2), i.e. 1 if n ≡ 1 (mod 4) and -1 if n ≡ 3
 *            (mod 4)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if ((n & 1) == 0) return (seq_t)0;          /* even composite (except n=2) */
    if (!is_prime(n)) return (seq_t)0;          /* odd composite */
    /* n is an odd prime */
    return (n % 4 == 1) ? (seq_t)1 : (seq_t)-1;
}
