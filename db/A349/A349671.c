/*
 * A349671  Number of iterations x -> (x+1)/2 needed to get 2 or a composite
 *          number, when starting with prime(n).
 * https://oeis.org/A349671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that ((prime(n)-1)/2^k)+1 is 2 or composite.
 *            Computed by repeatedly evaluating (x+1)/2 until the result is 2
 *            or composite
 * Range:     u8 n <= 54, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);          /* n-th prime */
    if (p <= 2) return 0;                   /* prime 2 needs 0 iterations */

    idx_t count = 0;
    while (p > 2 && is_prime((idx_t)p))
    {
        p = (p + 1) / 2;
        ++count;
    }
    return (seq_t)count;
}
