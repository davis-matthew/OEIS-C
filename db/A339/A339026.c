/*
 * A339026  Number of pairs (x,y): 1 <= x < y <= n*n, such that x*y is a
 *          square.
 * https://oeis.org/A339026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=2..n} phi(j) * floor(n^2/j^2)
 * Range:     u8 n <= 14, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes a(n) as defined above.
 * Maintains a cached array of Euler’s totient values up to the maximum
 * argument seen so far, recomputing when a larger n is requested. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* cached φ values */
    static idx_t phi_max = 0;
    static uint32_t *phi = NULL;

    if (n > phi_max) {
        uint32_t *new_phi = (uint32_t *)realloc(phi,
                               (size_t)(n + 1U) * sizeof(uint32_t));
        if (!new_phi)
            return 0;                 /* out‑of‑memory: return 0 as fallback */
        phi = new_phi;

        /* initialise: φ(i) = i */
        for (idx_t i = 0; i <= n; ++i)
            phi[i] = (uint32_t)i;

        /* sieve for φ */
        for (idx_t i = 2; i <= n; ++i) {
            if (phi[i] == (uint32_t)i) {          /* i is prime */
                for (idx_t j = i; j <= n; j += i)
                    phi[j] -= phi[j] / i;
            }
        }
        phi_max = n;
    }

    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t sum = 0;
    for (idx_t j = 2; j <= n; ++j) {
        seq_t jj = (seq_t)j * (seq_t)j;
        seq_t f  = n2 / jj;                /* floor(n^2 / j^2) */
        sum += (seq_t)phi[j] * f;
    }
    return sum;
}
