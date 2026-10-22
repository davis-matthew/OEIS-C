/*
 * A334065  Total area of all triangles such that p + q = 2*n, p < q (p, q
 *          prime), with base (q + p) and height (q - p).
 * https://oeis.org/A334065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n * Sum_{p<q, p+q=2*n} (n-p) where p and q are primes
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple primality test for n up to 2*10 000 (the largest value needed). */
static bool is_prime(idx_t x)
{
    if (x < 2)            return false;
    if (x == 2)           return true;
    if (x % 2 == 0)       return false;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* For n < 4 the value is 0 (no Goldbach partition with p < q). */
    if (n < 4) return (seq_t)0;

    seq_t total = 0;
    idx_t two_n = 2 * n;

    for (idx_t p = 2; p < n; ++p)
    {
        if (is_prime(p) && is_prime(two_n - p))
        {
            idx_t diff = n - p;           /* diff > 0 because p < n */
            total += (seq_t)two_n * (seq_t)diff;
        }
    }

    return total;
}
