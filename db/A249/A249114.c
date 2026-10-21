/*
 * A249114  Take the counting numbers and continue adding 1, 2, ..., a(n)
 *          until one reaches a fourth prime.
 * https://oeis.org/A249114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = minimal k>0 such that the set
   { n + 1, n + 1 + 2, n + 1 + 2 + 3, ... , n + 1 + 2 + ... + k }
   contains four primes.  The function returns that k. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;          /* current partial sum   */
    seq_t k   = 0;                 /* number of terms added */
    int    primecnt = 0;           /* primes seen so far    */

    while (primecnt < 4)
    {
        ++k;                       /* next term to add      */
        cur += (seq_t)k;           /* update partial sum    */
        if (is_prime_big(cur))
            ++primecnt;
    }
    return k;
}
