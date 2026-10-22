/*
 * A322038  Irregular triangle read by rows: for n >= 0, row n gives the
 *          coordination sequence for the tiling of a flat torus by a square
 *          grid with n points along each circuit.
 * https://oeis.org/A322038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322038 implementation
 *
 * The sequence is the flattened irregular triangle whose r‑th row
 * consists of the coordination sequence of the square torus Z²/(r·Z×r·Z).
 * Row r has length r+1 when r is even and length r when r is odd.
 *
 * The value at distance d (0 ≤ d ≤ maxd) is the number of vertices
 * at graph distance d from a reference vertex, which is the convolution
 * of the horizontal and vertical distance distributions.
 *
 * For even r = 2k:
 *   maxd = 2k and
 *   a(d) = 1                        if d = 0
 *          4d                        if 0 < d < k
 *          4k – 2                    if d = k
 *          4(2k – d)                if k < d < 2k
 *          1                        if d = 2k
 *
 * For odd r = 2k+1:
 *   maxd = 2k and
 *   a(d) = 1                        if d = 0
 *          4d                        if 0 < d ≤ k
 *          4(2k – d + 1)             if k < d ≤ 2k
 *
 * The function locates the row containing the linear index n by
 * accumulating row lengths until the remaining index lies inside
 * the current row, then applies the above formulas.
 *
 * Complexity is O(√n) per call, well under the 2.0 s limit for
 * indices up to 10000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* locate the row that contains the linear index n */
    idx_t row = 0;
    idx_t pos = n;               /* position inside the flattened array */

    /* row length L(row) = (row even) ? row+1 : row */
    while (1)
    {
        idx_t len = (row & 1) ? row : row + 1;
        if (pos < len)
            break;
        pos -= len;
        ++row;
    }

    /* compute the coefficient at position pos (distance d) in row row */
    if (row == 0)          /* first row is just 1 */
        return (seq_t)1;

    idx_t k = row >> 1;    /* floor(row/2) */
    idx_t d = pos;         /* distance within the row */

    if ((row & 1) == 0)     /* even row: row = 2k */
    {
        seq_t seq_d = (seq_t)d;
        seq_t seq_k = (seq_t)k;
        if (d == 0)                   return (seq_t)1;
        else if (d < k)               return (seq_t)(4 * seq_d);
        else if (d == k)              return (seq_t)(4 * seq_k - 2);
        else if (d < (k << 1))        return (seq_t)(4 * ((seq_t)(k << 1) - seq_d));
        else                          return (seq_t)1;       /* d == 2k */
    }
    else    /* odd row: row = 2k+1 */
    {
        seq_t seq_d = (seq_t)d;
        seq_t seq_k = (seq_t)k;
        if (d == 0)                   return (seq_t)1;
        else if (d <= k)              return (seq_t)(4 * seq_d);
        else                          return (seq_t)(4 * ((seq_t)(k << 1) - seq_d + 1));
    }
}
