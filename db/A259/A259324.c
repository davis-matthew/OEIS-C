/*
 * A259324  Infinite square array read by antidiagonals: T(n,k) = number of
 *          ways of partitioning numbers <= n into k parts (n >= 0, k >= 1).
 * https://oeis.org/A259324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(u,m) = T(u,m-1)+T(u-m,m), with T(0,m)=1, T(u,1)=u+1. The
 *            sequence a(n) is obtained by reading the values T(u,m) along
 *            antidiagonals of this infinite array
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* find the antidiagonal index d such that n lies in it */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n) {
        ++d;
    }

    /* position of n inside its diagonal */
    idx_t pos = n - d * (d + 1) / 2;

    /* the coordinates (u,m) of a(n) */
    idx_t u = pos;
    idx_t m = d - pos + 1;   /* m ranges from d+1 down to 1 */

    /* maximum dimensions needed for this call */
    idx_t max_u = d;          /* u ≤ d     */
    idx_t max_m = d + 1;      /* m ≤ d+1   */

    /* static DP table; 512 × 513 is more than enough for n <= 10 000
       (d ≈ 141 for n = 10 000). The table entries are overwritten on
       each call, which is fine because the size is tiny. */
    static seq_t T[512][513];

    /* compute the triangle up to the required size */
    for (idx_t ui = 0; ui <= max_u; ++ui) {
        for (idx_t mi = 1; mi <= max_m; ++mi) {
            if (ui == 0) {
                T[ui][mi] = (seq_t)1;
            } else if (mi == 1) {
                T[ui][mi] = (seq_t)ui + 1;
            } else {
                if (ui < mi) {
                    T[ui][mi] = T[ui][mi - 1];
                } else {
                    T[ui][mi] = T[ui][mi - 1] + T[ui - mi][mi];
                }
            }
        }
    }

    return T[u][m];
}
