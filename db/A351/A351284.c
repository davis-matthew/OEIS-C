/*
 * A351284  Irregular triangle read by rows: the n-th row gives the x-values
 *          of the solutions of the equation 2*(x - 1)*y - (x - 3)*x = 2*n for
 *          0 < x <= y.
 * https://oeis.org/A351284
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) derived from solutions of * 2*(x-1)*y - (x-3)*x = 2*n (0 <
 *            x <= y). * The sequence is the flattened irregular triangle: *
 *            for each n (starting at n = 3) the row contains all x‑values of
 *            * integer solutions (x,y) with x ≤ y, sorted in ascending order
 *            of x
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* fill 'out' with all x-values for a given row n.  
 * The values are written in ascending order of x.  
 * Returns the number of solutions. */
static size_t compute_row(idx_t n, seq_t *out)
{
    size_t len = 0;
    for (idx_t x = 2; ; ++x)
    {
        /* condition y >= x  <=>  x^2 + x <= 2*n */
        if (x * x + x > (idx_t)2 * n)
            break;

        seq_t numerator = (seq_t)2 * n + (seq_t)(x - 3) * x;
        seq_t denom     = (seq_t)2 * (x - 1);

        if (numerator % denom != 0)
            continue;

        seq_t y = numerator / denom;
        if (y < (seq_t)x)
            continue;           /* enforce y >= x */

        out[len++] = (seq_t)x;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 3. */
    if (n < 3)
        return 0;   /* undefined, return 0 for safety */

    /* L is the zero‑based index of the desired element in the
     * concatenated list of rows, starting from the first
     * element of row 3. */
    idx_t L   = n - 3;
    idx_t cum = 0;
    idx_t row = 3;

    /* Buffer large enough for all expected rows (max row length < 200 for
     * the tested range up to n = 10000). */
    seq_t buf[256];
    size_t len;

    while (1)
    {
        len = compute_row(row, buf);
        if (cum + len > L)
            break;          /* the desired element lies in this row */
        cum += len;
        ++row;
        /* guard against overflow (the problem constraints prevent it). */
        if (row == 0)
            break;
    }

    idx_t pos = L - cum;          /* zero‑based position within the row */
    return buf[(size_t)pos];
}
