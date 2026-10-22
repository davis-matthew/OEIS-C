/*
 * A308875  Irregular table read by rows: the n-th row lists the number of
 *          (n-1)-dimensional cells in the convex regular n-dimensional
 *          polytopes in ascending order, n >= 3.
 * https://oeis.org/A308875
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A308875 – Irregular table read by rows.
    Row 3: 4, 6, 8, 12, 20
    Row 4: 5, 8, 16, 24, 120, 600
    For n ≥ 5: row = sorted( n+1, 2·n, 2ⁿ )
    The sequence is obtained by concatenating the rows for
    dimensions n = 3, 4, 5, 6, …  */
#include "../utils/pow2.h"

static const seq_t row3[5] = { (seq_t)4, (seq_t)6, (seq_t)8, (seq_t)12, (seq_t)20 };
static const seq_t row4[6] = { (seq_t)5, (seq_t)8, (seq_t)16, (seq_t)24,
                               (seq_t)120, (seq_t)600 };

seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* behaviour below offset is irrelevant */

    idx_t offset = n - 3;                /* 0‑based position in the flattened sequence */

    /* Row 3 */
    if (offset < 5) return row3[offset];

    offset -= 5;
    /* Row 4 */
    if (offset < 6) return row4[offset];

    offset -= 6;                        /* offset now counts 3‑term blocks for n ≥ 5 */
    idx_t d   = 5 + offset / 3;          /* dimension of the current row (≥ 5) */
    idx_t pos = offset % 3;              /* position within the row */

    if (pos == 0) return (seq_t)(d + 1);
    if (pos == 1) return (seq_t)(2 * d);

    /* pos == 2 – the largest element 2^d */
    return pow2(d);                     /* pow2 is provided by the utils library */
}
