/*
 * A251191  Number of (n+2)X(6+2) 0..1 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A251191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0}^{2}∏_{j=0}^{2} C(r_i + c_j, r_i), * where r_i is
 *            the number of rows with index ≡ i (mod 3) among the (n+2) *
 *            rows, and c_j is the number of columns with index ≡ j (mod 3)
 *            among the * 8 columns
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 24, u128 n <= 136,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    const idx_t C = 8;               /* number of columns (6+2) */

    /* row counts r[i] for residues i = 0,1,2 */
    idx_t R = n + 2;                 /* number of rows */
    idx_t baseR = R / 3;
    idx_t remR  = R % 3;
    idx_t r[3] = {
        baseR + (remR >= 1 ? 1 : 0), /* residue 0 */
        baseR + (remR >= 2 ? 1 : 0), /* residue 1 */
        baseR                            /* residue 2 */
    };

    /* column counts c[j] for residues j = 0,1,2 */
    idx_t baseC = C / 3;
    idx_t remC  = C % 3;
    idx_t c[3] = {
        baseC + (remC >= 1 ? 1 : 0), /* residue 0 */
        baseC + (remC >= 2 ? 1 : 0), /* residue 1 */
        baseC                            /* residue 2 */
    };

    seq_t res = 1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t term = binom(r[i] + c[j], r[i]); /* binomial coefficient */
            res *= term;
        }
    }

    return res;
}
