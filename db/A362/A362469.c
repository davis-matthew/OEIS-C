/*
 * A362469  Sum of the numbers k, 1 <= k <= n, such that phi(k) | n.
 * https://oeis.org/A362469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n, phi(k) | n} k * = Sum_{k=1..n} k * [phi(k)
 *            divides n] (indicator function) * Computation: for each n,
 *            compute Euler’s totient φ(k) for all k ≤ n * via a sieve in O(n
 *            log log n) time, then sum those k * for which n % φ(k) == 0.
 *            Complexity O(n log log n) per call
 * Range:     u8 n <= 35, u16 n <= 1151, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* not used for the OEIS offset 1 */
    /* Allocate array for φ(0..n) */
    idx_t *phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!phi) return 0;                /* out‑of‑memory guard */

    /* Sieve of Eratosthenes for Euler’s totient */
    for (idx_t i = 1; i <= n; ++i) phi[i] = i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {             /* p is prime */
            for (idx_t m = p; m <= n; m += p) {
                phi[m] -= phi[m] / p;
            }
        }
    }

    /* Sum of k such that φ(k) divides n */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (n % phi[k] == 0) sum += (seq_t)k;
    }

    free(phi);
    return sum;
}
