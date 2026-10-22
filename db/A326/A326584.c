/*
 * A326584  a(n) = gcd(n*N(n-1), D(n-1)), with N(n)/D(n) = B(n) the n-th
 *          Bernoulli number.
 * https://oeis.org/A326584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( n, ∏_{p | n, (p-1) | (n-1)} p ) (uses the fact that
 *            for even k>0 the denominator of B_k is the product of primes p
 *            with p-1 | k, and the numerator is coprime to the denominator)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle n=1 directly (k=0) */
    if (n == 1ULL)
        return (seq_t)1;

    idx_t k = n - 1ULL;

    /* For odd k > 1 (i.e., n even > 2) the Bernoulli number is zero → a(n)=1 */
    if ((k & 1ULL) && k > 1ULL)
        return (seq_t)1;

    seq_t prod = 1;

    /* Factor out 2 first */
    if (n % 2ULL == 0ULL) {
        /* p-1 = 1 always divides k, so include 2 */
        prod *= (seq_t)2;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    }

    /* Factor odd primes */
    for (idx_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) {
            /* d is now a prime factor */
            if (k % (d - 1ULL) == 0ULL)
                prod *= (seq_t)d;
            while (n % d == 0ULL) n /= d;
        }
    }

    /* Remaining n > 1 is a prime factor */
    if (n > 1ULL) {
        if (k % (n - 1ULL) == 0ULL)
            prod *= (seq_t)n;
    }

    return prod;
}
