/*
 * A319087  a(n) = Sum_{k=1..n} k^2*phi(k), where phi is the Euler totient
 *          function A000010.
 * https://oeis.org/A319087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * phi(k)
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 409, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the Euler totient function for all integers up to n
   and then returns the sum of k^2 * phi(k) for k = 1..n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not needed for offset>=1 but safe */

    /* Allocate arrays for phi and accumulate sum */
    uint64_t *phi = (uint64_t*)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!phi) return (seq_t)0;  /* allocation failure: return 0 */

    /* Initialize phi[i] = i */
    for (idx_t i = 1; i <= n; ++i) phi[i] = (uint64_t)i;

    /* Sieve for phi */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {           /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi[j] -= phi[j] / p;
        }
    }

    /* Accumulate the sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t term = kk * kk * (seq_t)phi[k];
        sum += term;
    }

    free(phi);
    return sum;
}
