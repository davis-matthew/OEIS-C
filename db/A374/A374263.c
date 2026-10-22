/*
 * A374263  Number of distinct primitive Pythagorean triples (j^2 - k^2,
 *          2*j*k, j^2 + k^2) where 1 <= k < j <= n.
 * https://oeis.org/A374263
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=2..n} phi(j) if j even else phi(j)/2 (where phi is
 *            Euler's totient function). This counts the number of distinct
 *            primitive Pythagorean triples (j²-k², 2jk, j²+k²) with 1 ≤ k < j
 *            ≤ n
 * Range:     u8 n <= 35, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* offset starts at n = 2 */

    /* Allocate phi array of size n+1 (indices 0..n) */
    idx_t *phi = (idx_t*)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!phi) return 0;  /* allocation failed */

    /* Initialisation: phi[i] = i */
    for (idx_t i = 0; i <= n; ++i) {
        phi[i] = i;
    }

    /* Euler's totient sieve */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {          /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi[j] = (phi[j] / p) * (p - 1);
            }
        }
    }

    /* Compute cumulative sum of primitive triples */
    seq_t acc = 0;
    for (idx_t j = 2; j <= n; ++j) {
        seq_t contrib = (seq_t)phi[j];
        if (j & 1) {                /* j odd → only half of coprime k's work */
            contrib /= 2;           /* phi[j] is even for odd j>1 */
        }
        acc += contrib;
    }

    free(phi);
    return acc;
}
