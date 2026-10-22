/*
 * A341829  Irregular triangle read by rows: the n-th row gives the x-values
 *          of the solutions of the equation x*(y - 1) + (2*x - y - 1)*(x mod
 *          2) = 2*n for 0 < x <= y.
 * https://oeis.org/A341829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9661, u16 n <= 9661, u32 n <= 9661, u64 n <= 9661,
 *            u128 n <= 9661, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A341829: Enumerate all solutions (x,y) of
 *     x*(y-1) + (2*x - y - 1)*(x mod 2) = 2*n     (0 < x <= y)
 * Each row n contains the x-values of the solutions sorted
 * in increasing order.  The overall sequence a(k) is the row
 * values read by rows (row 1, then row 2, etc.).
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort on seq_t */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    return (*a > *b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n < 1 */

    idx_t cum = 0;                 /* total number of terms seen so far */
    idx_t row = 1;                 /* current row (n in the equation) */

    while (1)
    {
        idx_t m = row * 2;         /* RHS of the equation */
        seq_t buf[256];
        size_t len = 0;

        /* Enumerate all divisors d of m */
        for (idx_t d = 1; d <= m / d; ++d)
        {
            if (m % d != 0) continue;

            /* Even x: x == d or x == m/d */
            idx_t x = d;
            if ((x & 1) == 0) {
                idx_t y = 1 + m / x;
                if (y >= x) buf[len++] = (seq_t)x;
            }
            idx_t d2 = m / d;
            if (d2 != d) {
                x = d2;
                if ((x & 1) == 0) {
                    idx_t y = 1 + m / x;
                    if (y >= x) buf[len++] = (seq_t)x;
                }
            }

            /* Odd x: x == d+1 or x == d2+1 */
            x = d + 1;
            if ((x & 1) == 1) {
                idx_t y = m / d - 1;
                if (y >= x) buf[len++] = (seq_t)x;
            }
            if (d2 != d) {
                x = d2 + 1;
                if ((x & 1) == 1) {
                    idx_t y = m / d2 - 1;
                    if (y >= x) buf[len++] = (seq_t)x;
                }
            }
        }

        if (cum + len >= n) {
            /* The desired term lies in this row */
            size_t offset = n - cum - 1;
            if (len > 1) qsort(buf, len, sizeof(seq_t), cmp_seq_t);
            return buf[offset];
        }

        cum += len;
        ++row;
    }
}
