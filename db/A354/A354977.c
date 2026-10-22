/*
 * A354977  Triangle read by rows. T(n, k) =
 *          Sum_{j=0..n}((-1)^(n-j)*binomial(n, j)*j^(n+k)) / n!.
 * https://oeis.org/A354977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, and T(r,c) = Stirling2(r+c,r)
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_sqrt, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Determine the row index r and column offset c for position n. */
    seq_t tmp_val = (seq_t)8 * (seq_t)n + (seq_t)1;
    seq_t tmp = floor_sqrt(tmp_val);
    idx_t r = ((idx_t)tmp - (idx_t)1) / 2;

    idx_t cumulativePrev = r * (r + 1) / 2;
    idx_t offset = n - cumulativePrev;
    idx_t m = r + offset;   /* m = r + c = (row + column) */

    /* Compute Stirling numbers of the second kind S(m, r) via DP. */
    seq_t *s = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!s) return (seq_t)0;
    for (idx_t i = 0; i <= m; ++i) s[i] = (seq_t)0;
    s[0] = (seq_t)1;   /* S(0,0)=1 */

    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            seq_t prod = mul_small(s[j], (unsigned int)j);
            s[j] = prod + s[j - 1];
        }
        s[0] = (seq_t)0;   /* S(i,0)=0 for i>0 */
    }

    seq_t res = s[r];
    free(s);
    return res;
}
