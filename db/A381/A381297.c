/*
 * A381297  a(n) = numerator( [x^n] hypergeom([1/2, 1/2, 1/2, 1/4, 3/4], [1,
 *          1, 1, 1], 256*x) ).
 * https://oeis.org/A381297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( 4*(4*n - 1)!*Gamma(n+1/2)^2/(Pi*(n-1)!*(n!)^5)
 *            )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

static inline idx_t exp_factorial(idx_t k, idx_t p) {
    idx_t t = k;
    idx_t sum = 0;
    while (t) {
        t /= p;
        sum += t;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t max = 4 * n;
    /* Sieve of Eratosthenes up to max */
    char *is_prime = (char *)malloc((size_t)(max + 1));
    if (!is_prime) return 0;
    for (idx_t i = 0; i <= max; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= max; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= max; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    seq_t result = 1;
    idx_t k2 = 2 * n;
    idx_t k4 = 4 * n;
    for (idx_t p = 2; p <= max; ++p) {
        if (!is_prime[p]) continue;
        long long e2n = exp_factorial(k2, p);
        long long e4n = exp_factorial(k4, p);
        long long en  = exp_factorial(n, p);
        long long exp = 2 * e2n + e4n - 8 * en;
        if (p == 2) exp -= 4 * n;
        if (exp > 0) {
            seq_t pow_part = seq_pow((seq_t)p, (idx_t)exp);
            result *= pow_part;
        }
    }

    free(is_prime);
    return result;
}
