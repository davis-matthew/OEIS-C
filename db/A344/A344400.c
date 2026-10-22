/*
 * A344400  a(n) = [x^n] 6*(8*x^3 + 3*x + 1) / (1 - 4*x)^(7/2).
 * https://oeis.org/A344400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] 6*(8*x^3 + 3*x + 1) / (1 - 4*x)^(7/2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Compute g_k = coefficient of x^k in (1-4x)^(-7/2),
       using the recurrence g_{k+1} = (4k+14)/(k+1) * g_k, with g_0 = 1. */
    seq_t *g = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!g) return 0;                      /* allocation failed – return 0 */

    g[0] = 1;                              /* g_0 = 1 */
    for (idx_t i = 0; i < n; ++i) {
        seq_t num = (seq_t)(4 * i + 14) * g[i];
        seq_t den = i + 1;
        g[i + 1] = num / den;              /* exact division by construction */
    }

    seq_t gn   = g[n];
    seq_t gn1  = (n >= 1) ? g[n - 1] : 0;  /* g_{n-1}, zero if n<1   */
    seq_t gn3  = (n >= 3) ? g[n - 3] : 0;  /* g_{n-3}, zero if n<3   */

    seq_t result = 6 * (gn + 3 * gn1 + 8 * gn3);

    free(g);
    return result;
}
