/*
 * A341918  Number of strong triangulations of a fixed hexagon with n interior
 *          nodes.
 * https://oeis.org/A341918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120*(67*n^2 + 138*n + 68)*(4*n+7)!/((3*n+12)!*(n-1)!)
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   legendre_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(1) = 1, a(2) = 15, a(3) = 155, a(4) = 1410, ... */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/legendre_sum.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard; offset is 1 */

    /* compute M = 4*n+7, N = 3*n+12, K = n-1 */
    idx_t M = 4 * n + 7;
    idx_t N = 3 * n + 12;
    idx_t K = n - 1;

    /* prefactor = 120*(67*n^2 + 138*n + 68) */
    seq_t prefactor = (seq_t)120;
    prefactor *= ((seq_t)67 * n * n + (seq_t)138 * n + (seq_t)68);

    /* limit for sieve: max(M,N) (for n<5 N > M) */
    idx_t limit = (M > N) ? M : N;
    if (limit < 2) limit = 2;

    /* sieve of Eratosthenes up to limit */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return 0;
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }

    /* collect primes into array */
    idx_t primes_capacity = limit / 10 + 10;       /* rough upper bound */
    idx_t *primes = (idx_t *)malloc(primes_capacity * sizeof(idx_t));
    size_t primes_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            if (primes_count == primes_capacity) {
                primes_capacity *= 2;
                primes = (idx_t *)realloc(primes, primes_capacity * sizeof(idx_t));
            }
            primes[primes_count++] = i;
        }
    free(is_prime);

    /* exponent array (signed) */
    int64_t *exp = (int64_t *)calloc(primes_count, sizeof(int64_t));
    if (!exp) {
        free(primes);
        return 0;
    }

    /* compute exponent difference from factorials */
    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        exp[i] = (int64_t)legendre_sum(M, p)
                 - (int64_t)legendre_sum(N, p)
                 - (int64_t)legendre_sum(K, p);
    }

    /* factor prefactor and adjust exponents */
    seq_t temp = prefactor;
    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        seq_t sp = (seq_t)p;
        while (temp % sp == 0) {
            temp /= sp;
            exp[i] += 1;
        }
    }

    /* multiply primes raised to positive exponents */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < primes_count; ++i) {
        while (exp[i] > 0) {
            result *= (seq_t)primes[i];
            exp[i] -= 1;
        }
    }

    /* any remaining factor >1 is a product of primes > limit */
    if (temp > 1) {
        result *= temp;
    }

    /* cleanup */
    free(primes);
    free(exp);

    return result;
}
