/*
 * A293975  If n is even, divide by 2; otherwise, add the next larger prime.
 * https://oeis.org/A293975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 if n is even; otherwise a(n) = n + next_prime(n)
 * Range:     u8 n <= 126, u16 n <= 9706, u32 n <= 9706, u64 n <= 9706,
 *            u128 n <= 9706, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (next_prime(n) returns the least prime strictly larger than n)      */

#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0)          /* n even */
        return (seq_t)(n / 2);
    else                          /* n odd */
    {
        uint64_t np = next_prime(n);   /* next prime > n */
        return (seq_t)n + (seq_t)np;
    }
}
