/*
 * A248822  Number of integers k^4 that divide 1!*2!*3!*...*n!.
 * https://oeis.org/A248822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} (⌊v_p(n)/4⌋ + 1), where v_p(n) = Σ_{i=1}^n ⌊i/p⌋
 *            + ⌊i/p²⌋ +
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 30, u64 n <= 55, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of integer fourth powers dividing the product 1!*2!*…*n! */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* out of offset, return 0 */

    /* Special case: n == 1, no primes to consider */
    if (n == 1)
        return 1;

    /* ---------- Sieve primes up to n ---------- */
    size_t sieve_size = (size_t)n + 1;
    bool *is_prime = (bool *)malloc(sieve_size);
    if (!is_prime)
        return 0;                           /* allocation failed */

    for (size_t i = 0; i < sieve_size; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= (size_t)n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_prime[j] = false;
        }
    }

    /* Collect primes into a dynamic array */
    size_t max_cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i])
            ++max_cnt;

    uint64_t *primes = (uint64_t *)malloc(max_cnt * sizeof(uint64_t));
    size_t pcnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i])
            primes[pcnt++] = (uint64_t)i;

    free(is_prime);

    /* ---------- Compute a(n) ---------- */
    seq_t result = 1;

    for (size_t pi = 0; pi < pcnt; ++pi) {
        uint64_t p = primes[pi];
        seq_t vp = 0;                        /* exponent of p in the product */

        idx_t k = p;
        while (k <= n) {
            idx_t m = n / k;                 /* floor(n / k) */
            if (m == 0)
                break;

            /* S(n,k) = Σ_{i=1}^n ⌊i/k⌋ = k*(m-1)*m/2 + m*(n - m*k + 1) */
            seq_t term1 = ((seq_t)k * (seq_t)(m - 1) * (seq_t)m) / 2;
            seq_t term2 = (seq_t)m * (seq_t)(n - m * k + 1);
            vp += term1 + term2;

            if (k > n / p)                   /* avoid overflow of k *= p */
                break;
            k *= p;
        }

        seq_t factor = vp / 4 + 1;
        result *= factor;
    }

    free(primes);
    return result;
}
