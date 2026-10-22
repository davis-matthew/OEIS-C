/*
 * A334158  Number of Goldbach partitions (p,q) of 2n such that primes p and q
 *          can also be written as the sum of two primes.
 * https://oeis.org/A334158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} c(i)*c(i-2)*c(2*n-i)*c(2*n-i-2) * where
 *            c(k)=1 if k is prime else 0. (Goldbach partitions where both *
 *            primes can be expressed as sum of two primes.)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t limit = 2 * n;
    bool *prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime) return 0;               /* allocation failure */

    for (idx_t i = 0; i <= limit; ++i) prime[i] = true;
    prime[0] = false;
    if (limit >= 1) prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                prime[q] = false;
        }
    }

    seq_t count = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (prime[i] && prime[i - 2] &&
            prime[2 * n - i] && prime[2 * n - i - 2]) {
            ++count;
        }
    }

    free(prime);
    return count;
}
