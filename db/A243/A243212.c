/*
 * A243212  Number of ways to place 3 points on a triangular grid of side n so
 *          that no three of them are vertices of an equilateral triangle with
 *          sides parallel to the grid.
 * https://oeis.org/A243212
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n*(n+1)/2, 3) – floor((n-1)*(n+1)*(2*n-1)/8) (OEIS
 *            A243212)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   choose3, sub_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/choose3.h"
#include "../utils/sub_term.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;                /* outside the official offset – undefined */

    /* N = n*(n+1)/2  (computed with early division to avoid overflow) */
    seq_t sn = (seq_t)n;
    seq_t N;
    if (sn % 2 == 0)
        N = (sn / 2) * (sn + 1);
    else
        N = sn * ((sn + 1) / 2);

    seq_t comb = choose3(N);
    seq_t sub  = sub_term(n);
    return comb - sub;
}
