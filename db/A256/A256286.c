/*
 * A256286  Number of Hamiltonian cycles in a tournament on 3n vertices
 *          constructed by taking 3 copies of a transitive tournament on n
 *          vertices and placing each copy on a vertex of a directed 3-cycle,
 *          with all edges between the copies oriented in the direction of the
 *          cycle.
 * https://oeis.org/A256286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (S(n,k)*k!)^3 / k, where S(n,k) are Stirling
 *            numbers of the second kind
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate arrays for Stirling numbers and factorials. */
    idx_t cap = n + 1;
    seq_t *S   = (seq_t *)malloc(sizeof(seq_t) * (size_t)cap);
    seq_t *fac = (seq_t *)malloc(sizeof(seq_t) * (size_t)cap);
    if (!S || !fac) {
        free(S);
        free(fac);
        return 0;
    }

    /* Initialise Stirling numbers: S(0,0)=1, all others 0. */
    for (idx_t i = 0; i <= n; ++i) S[i] = 0;
    S[0] = 1;

    /* Precompute factorials up to n. */
    fac[0] = 1;
    for (idx_t k = 1; k <= n; ++k)
        fac[k] = fac[k - 1] * (seq_t)k;

    seq_t sum = 0;

    /* Build Stirling numbers row‑by‑row until row n, then compute sum. */
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = m; k >= 1; --k) {
            S[k] = S[k - 1] + (seq_t)k * S[k];
        }
        S[0] = 0;                 /* S(m,0)=0 for m>0 */

        if (m == n) {
            for (idx_t k = 1; k <= n; ++k) {
                seq_t val   = S[k] * fac[k];
                seq_t term  = val * val * val / (seq_t)k;
                sum += term;
            }
        }
    }

    free(S);
    free(fac);
    return sum;
}
