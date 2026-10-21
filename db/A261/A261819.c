/*
 * A261819  Encoded symmetrical antidiagonal square binary matrices with
 *          either 1 or 2 ones.
 * https://oeis.org/A261819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A261195(2^n) – the binary value obtained by scanning the
 *            * antidiagonal of a square binary matrix whose antidiagonal is
 *            symmetric * and contains either one or two ones. The mapping
 *            from the index n to the * matrix size m and the pattern of ones
 *            is described in the code
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   count_for_size, idx_of_cell, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/count_for_size.h"
#include "../utils/idx_of_cell.h"

/* Main sequence function.                                               */
seq_t a(idx_t n)
{
    /* Find the matrix size m and the position r inside the block. */
    idx_t m   = 1;
    idx_t cum = 0;
    while (cum + count_for_size(m) <= n) {
        cum += count_for_size(m);
        ++m;
    }
    idx_t r = n - cum;   /* 0‑based index within the block of size m */

    if (m & 1) {                     /* odd size – centre + pairs          */
        if (r == 0) {                /* single centre one                  */
            idx_t c   = (m - 1) >> 1;
            idx_t exp = idx_of_cell(m, c, c);
            return seq_pow(2, exp);
        }
        /* two symmetric ones, distance d = r from the centre */
        idx_t d   = r;
        idx_t c   = (m - 1) >> 1;
        idx_t i1  = c - d, j1 = c + d;
        idx_t i2  = c + d, j2 = c - d;
        idx_t exp1 = idx_of_cell(m, i1, j1);
        idx_t exp2 = idx_of_cell(m, i2, j2);
        seq_t v1 = seq_pow(2, exp1);
        seq_t v2 = seq_pow(2, exp2);
        return v1 + v2;
    } else {                          /* even size – always two ones       */
        idx_t outer = m >> 1;          /* number of symmetric pairs        */
        idx_t d = outer - 1 - r;       /* inner‑most first, outer‑most last*/
        idx_t i1  = d, j1 = m - 1 - d;
        idx_t i2  = m - 1 - d, j2 = d;
        idx_t exp1 = idx_of_cell(m, i1, j1);
        idx_t exp2 = idx_of_cell(m, i2, j2);
        seq_t v1 = seq_pow(2, exp1);
        seq_t v2 = seq_pow(2, exp2);
        return v1 + v2;
    }
}
