/*
 * A389838  Numbers of the form j*k or j*k + (j-1)*(k-1) with j, k > 1.
 * https://oeis.org/A389838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A389838
 *  Formula: a(n) = the n-th integer ≥4 that is NOT a prime p with 2p‑1 also prime.
 *
 *  A number appears if it is
 *    (i) composite (j*k with j>1, k>1)  OR
 *    (ii) a prime p for which 2p‑1 is composite.
 *
 *  The missing numbers are exactly the primes p for which 2p‑1 is prime.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* the offset is 1;  a(0) is unused */
        return (seq_t)0;

    idx_t count = 0;           /* count of valid numbers seen */
    for (idx_t i = 4; ; ++i)   /* start from 4, the first possible value */
    {
        if (is_prime(i))          /* if i is prime, test the second condition */
        {
            if (is_prime(2ULL * i - 1))
                continue;         /* skip this prime – 2p‑1 is also prime */
        }

        ++count;                  /* i is a valid term of the sequence */
        if (count == n)
            return (seq_t)i;
    }
}
