/*
 * A346621  a(n) = Sum_{ x <= n : omega(x) = 2 } x.
 * https://oeis.org/A346621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x <= n , omega(x)=2} x
 * Range:     u8 n <= 33, u16 n <= 499, u32 n <= 9406, u64 n <= 9406,
 *            u128 n <= 9406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* Array to store the number of distinct prime factors of each integer up to n */
    unsigned char *omega = (unsigned char*)calloc((size_t)(n + 1), sizeof(unsigned char));
    if (!omega) return (seq_t)0;   /* allocation failed */

    /* Sieve: for every prime p increment omega[j] for all multiples j of p */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (omega[p] == 0)   /* p is prime */
        {
            for (idx_t j = p; j <= n; j += p)
                omega[j]++; 
        }
    }

    seq_t sum = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        if (omega[i] == 2)
            sum += (seq_t)i;
    }

    free(omega);
    return sum;
}
