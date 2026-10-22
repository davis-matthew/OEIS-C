/*
 * A368616  a(n) = Sum_{k=1..n} pi(k) * (ceiling(n/k) - floor(n/k)).
 * https://oeis.org/A368616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} π(k)·(⌈n/k⌉ – ⌊n/k⌋) = Σ_{k≤n} π(k) – Σ_{d|n}
 *            π(d)
 * Range:     u8 n <= 38, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence using a sieve to obtain π(k)
   for all k ≤ n, then subtracts π(d) for each divisor d of n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate arrays: isPrime (bool) and pi(k) (seq_t) */
    bool *isPrime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    seq_t *pi_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!isPrime || !pi_arr) {
        free(isPrime);
        free(pi_arr);
        return 0;
    }

    /* Initialize isPrime array: all true except 0 and 1 */
    for (idx_t i = 0; i <= n; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;

    /* Sieve of Eratosthenes up to n */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (isPrime[p]) {
            for (idx_t j = p * p; j <= n; j += p) {
                isPrime[j] = false;
            }
        }
    }

    /* Compute pi(k) array and sum_{k≤n} π(k) */
    seq_t cumPi = 0;          /* current π(k) */
    seq_t totalSum = 0;       /* Σ_{k≤n} π(k) */
    pi_arr[0] = 0;
    pi_arr[1] = 0;
    for (idx_t k = 2; k <= n; ++k) {
        if (isPrime[k]) ++cumPi;
        pi_arr[k] = cumPi;
        totalSum += cumPi;
    }

    /* Sum π(d) over all divisors d of n */
    seq_t divSum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divSum += pi_arr[d];
            if (d != n / d) divSum += pi_arr[n / d];
        }
    }

    /* Free allocated memory */
    free(isPrime);
    free(pi_arr);

    /* Result: Σ_{k≤n} π(k) – Σ_{d|n} π(d) */
    return totalSum - divSum;
}
