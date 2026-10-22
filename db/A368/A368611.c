/*
 * A368611  a(n) = Sum_{k=1..n} pi(k) * floor(n/k).
 * https://oeis.org/A368611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} pi(k) * floor(n/k)
 * Range:     u8 n <= 27, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Sieve of Eratosthenes up to n */
    bool *prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!prime)
        return 0;
    for (idx_t i = 0; i <= n; i++)
        prime[i] = true;
    if (n >= 0)
        prime[0] = false;
    if (n >= 1)
        prime[1] = false;
    for (idx_t i = 2; i * i <= n; i++) {
        if (prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                prime[j] = false;
        }
    }

    /* Prefix array of π(k) */
    seq_t *pi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pi) {
        free(prime);
        return 0;
    }
    pi[0] = 0;
    for (idx_t i = 1; i <= n; i++)
        pi[i] = pi[i - 1] + (prime[i] ? 1 : 0);

    /* Compute the sum Σ_{k=1..n} π(k) * ⌊n/k⌋ */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        seq_t pk = pi[k];
        seq_t t = (seq_t)(n / k);
        result += pk * t;
    }

    free(prime);
    free(pi);
    return result;
}
