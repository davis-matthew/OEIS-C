/*
 * A395566  Sum of the distinct prime divisors of C(6*n, n).
 * https://oeis.org/A395566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopf(binomial(6*n, n)) Computes the sum of the distinct
 *            prime divisors of C(6*n, n)
 * Range:     u8 n <= 11, u16 n <= 238, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Exponent of prime p in m!   (m and p are unsigned 64‑bit). */
static unsigned long long exp_in_factorial(idx_t m, uint64_t p)
{
    unsigned long long e = 0;
    idx_t div = m / p;
    while (div > 0) {
        e += div;
        div /= p;
    }
    return e;
}

/* Main sequence function.  idx_t is unsigned 64‑bit */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, return 0 */
        return 0;

    idx_t limit = 6 * n;          /* maximum prime needed */

    /* Simple Sieve of Eratosthenes up to 'limit' */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)        /* allocation failure – fall back to 0 */
        return 0;

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    seq_t sum = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            unsigned long long e = exp_in_factorial(6 * n, (uint64_t)p)
                                 - exp_in_factorial(n,      (uint64_t)p)
                                 - exp_in_factorial(5 * n,  (uint64_t)p);
            if (e > 0)
                sum += (seq_t)p;
        }
    }

    free(is_prime);
    return sum;
}
