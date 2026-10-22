/*
 * A303231  Total volume of all rectangular prisms with dimensions q, p+q and
 *          |q-p| such that p and q are prime, n = p+q and p < q.
 * https://oeis.org/A303231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{i=1..floor((n-1)/2)} (n-i)*(n-2*i)*c(i)*c(n-i)
 *            where c(k) = 1 if k is prime else 0
 * Range:     u8 n <= 8, u16 n <= 39, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple primality test suitable for n up to about 10^12 (and far beyond
   for the use in this sequence).  It uses trial division up to sqrt(n). */
static bool is_prime_local(idx_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (idx_t d = 3; d <= m / d; d += 2)
    {
        if (m % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over i from 1 to floor((n-1)/2) */
    idx_t limit = (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i)
    {
        if (is_prime_local(i) && is_prime_local(n - i))
        {
            seq_t part1 = (seq_t)(n - i);                      /* n - i  */
            seq_t part2 = (seq_t)n - ((seq_t)2 * (seq_t)i);   /* n - 2*i */
            sum += part1 * part2;
        }
    }
    return (seq_t)n * sum;
}
