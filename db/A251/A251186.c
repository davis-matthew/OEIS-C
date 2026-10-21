/*
 * A251186  Number of (n+2)X(n+2) 0..1 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A251186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{r=0}^{2} ∏_{s=0}^{2} binom( count(r) + count(s),
 *            count(r) ) where count(r) = ⌊(n+2)/3⌋ + [ (n+2)%3 ≥ r ? 1 : 0 ]
 *            for r=0,1,2 (rows and columns are counted with indices 1 … n+2,
 *            i.e. the matrix is (n+2)x(n+2)). The formula follows from the
 *            observation that in every 3‑consecutive window t...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = n + 2;                /* matrix dimension */
    idx_t q = m / 3;
    idx_t r = m % 3;

    /* counts of rows/columns with residues 0,1,2 (mod 3) */
    idx_t c[3];
    c[0] = q;
    c[1] = q + (r >= 1);
    c[2] = q + (r >= 2);

    seq_t ans = (seq_t)1;

    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            ans *= binom(c[i] + c[j], c[i]);

    return ans;
}
