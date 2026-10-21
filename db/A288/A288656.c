/*
 * A288656  a(n) = Sum_{k=1..n} Sum_{i=floor((k-1)/2)..k-1} i * c(i), where c
 *          is the prime characteristic (A010051).
 * https://oeis.org/A288656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} Sum_{i=floor((k-1)/2)..k-1} i * c(i), where
 *            c(i) = 1 if i is prime, 0 otherwise. This implementation
 *            precomputes a prime sieve and prefix sums of primes up to n,
 *            then evaluates the double sum in O(n) time
 * Range:     u8 n <= 17, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Allocate array for prime flags and prefix sums */
    bool *is_prime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;                /* fallback on allocation failure */

    seq_t *pref = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pref) {
        free(is_prime);
        return (seq_t)0;
    }

    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = (i >= 2);
    for (idx_t p = 2; p * p <= n; ++p) {
        if (!is_prime[p])
            continue;
        for (idx_t m = p * p; m <= n; m += p)
            is_prime[m] = false;
    }

    /* Prefix sums of primes */
    pref[0] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        pref[i] = pref[i - 1] + (is_prime[i] ? (seq_t)i : (seq_t)0);
    }

    /* Compute the double sum */
    seq_t total = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t L = (k - 1) / 2;
        idx_t R = k - 1;
        seq_t sum = pref[R];
        if (L > 0)
            sum -= pref[L - 1];
        total += sum;
    }

    free(is_prime);
    free(pref);
    return total;
}
