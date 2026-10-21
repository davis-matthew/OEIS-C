/*
 * A290131  Number of regions in a regular drawing of the complete bipartite
 *          graph K_{n,n}.
 * https://oeis.org/A290131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)^2 + Sum_{i=2..n-1} (n-i)*(2n-i)*phi(i)
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 410, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (with a(1) = 0) */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0 */
        return 0;

    seq_t nm1 = (seq_t)(n - 1);        /* n-1 as seq_t */
    seq_t sn   = (seq_t)n;             /* n as seq_t */
    seq_t base = 2 * nm1 * nm1;        /* 2*(n-1)^2 */
    seq_t sum  = 0;

    for (idx_t i = 2; i < n; ++i) {
        seq_t term1 = sn - (seq_t)i;                      /* n-i  */
        seq_t term2 = 2 * sn - (seq_t)i;                  /* 2n-i */
        seq_t phi_i = phi(i);                              /* φ(i) */
        sum += term1 * term2 * phi_i;
    }

    return base + sum;
}
