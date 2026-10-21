/*
 * A257743  Let n'= odd part of n. If n' is 1 or prime, then a(n)=1; otherwise
 *          a(n) is 1+ the number of times the map x -> (3*x+1)' must be
 *          applied to n' in order to reach 1 or a prime.
 * https://oeis.org/A257743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of iterations of x -> (3*x+1)' applied to n'
 *            until the result is 1 or a prime, where n' is n with all factors
 *            of 2 removed
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* compute odd part of n */
    idx_t x = n;
    while ((x & 1ULL) == 0)
        x >>= 1;

    seq_t steps = 0;
    while (x != 1 && !is_prime(x)) {
        unsigned __int128 tmp = (unsigned __int128)3 * x + 1;
        /* strip factors of 2 */
        while ((tmp & 1) == 0)
            tmp >>= 1;
        x = (idx_t)tmp;          /* tmp fits in idx_t for n up to reasonable size */
        steps += 1;
    }
    return steps + (seq_t)1;
}
