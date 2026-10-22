/*
 * A396021  Sum of the distinct prime divisors of C(10*n, n).
 * https://oeis.org/A396021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct prime divisors of C(10*n, n)
 * Range:     u8 n <= 8, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below works for any seq_t and idx_t provided by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* not part of the sequence (index 0) */

    idx_t limit = 10ULL * n;            /* 10*n */
    idx_t nine_n = limit - n;           /* 9*n */

    /* Allocate a boolean array for the sieve */
    size_t size = (size_t)(limit + 1);
    char *is_prime = (char *)malloc(size);
    if (!is_prime)
        return 0;                       /* allocation failure */

    memset(is_prime, 1, size);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    seq_t sum = 0;

    /* For every prime p ≤ limit, compute its exponent in C(10*n, n) */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;

        idx_t power = p;
        int exp = 0;
        while (power <= limit) {
            exp += (int)(limit / power)
                 - (int)(n / power)
                 - (int)(nine_n / power);
            if (power > limit / p)   /* prevent overflow of power *= p */
                break;
            power *= p;
        }

        if (exp > 0)
            sum += (seq_t)p;          /* add distinct prime divisor */
    }

    free(is_prime);
    return sum;
}
