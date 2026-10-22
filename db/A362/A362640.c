/*
 * A362640  Product of the larger primes, q, in the Goldbach partitions of 2n
 *          such that p + q = 2n, p <= q, and p,q prime (or 1 if no Goldbach
 *          partition of 2n exists).
 * https://oeis.org/A362640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏{q | p+q = 2n, p ≤ q, p and q prime} q (product of the
 *            larger primes in the Goldbach partitions of 2n; 1 if none)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* compute a(n) as defined above */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* offset starts at 1, undefined for n==0 */

    seq_t prod = 1;
    /* iterate over potential smaller primes p, from 2 up to n */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (!is_prime(p))
            continue;

        idx_t q = 2 * n - p;           /* larger complement in the Goldbach sum */
        if (q < p)                     /* further p will only make q smaller */
            break;

        if (is_prime(q))
            prod = (seq_t)prod * (seq_t)q;
    }
    return prod;
}
