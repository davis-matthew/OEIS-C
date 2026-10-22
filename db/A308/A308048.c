/*
 * A308048  a(n) = n - nextprime(ceiling(n/2) - 1), where nextprime(n) is the
 *          smallest prime > n.
 * https://oeis.org/A308048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - nextprime(ceil(n/2) - 1)
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    idx_t start = (n + 1) / 2 - 1;        /* ceil(n/2) - 1 */
    uint64_t p = next_prime(start);      /* smallest prime > start */
    return (seq_t)n - (seq_t)p;
}
