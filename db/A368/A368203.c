/*
 * A368203  Least k such that 7*n*k+1 is a prime.
 * https://oeis.org/A368203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 7*n*k+1 is prime
 * Range:     u8 n <= 10, u16 n <= 1180, u32 n <= 1572, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is indexed from 1; no need to handle n==0, but guard against it */
    if (n == 0)
        return 0;

    idx_t k = 1;
    while (1)
    {
        seq_t candidate = (seq_t)7 * (seq_t)n * (seq_t)k + 1;
        if (is_prime_big(candidate))
            return (seq_t)k;
        k++;
    }
}
