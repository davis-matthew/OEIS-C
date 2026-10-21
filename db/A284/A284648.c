/*
 * A284648  Numerator of sum of reciprocals of all divisors of all positive
 *          integers <= n.
 * https://oeis.org/A284648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=1..n} sigma(k)/k
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* 1. Sieve of Eratosthenes to obtain all primes ≤ n */
    idx_t limit = (n < 2) ? 2 : n;
    bool *is_comp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) return 0;
    memset(is_comp, 0, (limit + 1) * sizeof(bool));

    /* allocate enough space for primes; at most limit/2 primes */
    idx_t *primes = (idx_t *)malloc((limit / 2 + 1) * sizeof(idx_t));
    if (!primes) { free(is_comp); return 0; }

    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* 2. Compute L = lcm(1..n) as product of maximal prime powers */
    seq_t L = 1;
    for (idx_t pi = 0; pi < prime_cnt; ++pi) {
        seq_t p = primes[pi];
        seq_t pk = p;
        while (pk * p <= n) {
            pk *= p;
        }
        L *= pk;
    }
    free(primes);

    /* 3. Compute N = Σ_{d=1..n} floor(n/d) * (L / d) */
    seq_t N = 0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t m = (seq_t)(n / d);           /* floor(n/d) */
        seq_t term = (L / (seq_t)d) * m;     /* (L/d) * m */
        N += term;
    }

    /* 4. Reduce to obtain the numerator */
    seq_t g = gcd(N, L);
    return N / g;
}
