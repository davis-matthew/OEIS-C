/*
 * A341830  Irregular triangle read by rows: the n-th row gives the y-values
 *          of the solutions of the equation x*(y - 1) + (2*x - y - 1)*(x mod
 *          2) = 2*n for 0 < x <= y.
 * https://oeis.org/A341830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by iterating rows of solutions to the equation
 *            x*(y – 1)+(2*x – y – 1)*(x mod 2)=2*n for 0 < x ≤ y, collecting
 *            all y‑values, sorting them ascending, and concatenating the
 *            rows. The implementation finds the n‑th element by generating
 *            rows one by one until the cumulative count reac...
 * Range:     u8 n <= 1277, u16 n <= 9766, u32 n <= 9766, u64 n <= 9766,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Comparator for qsort, working for any unsigned seq_t type. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Collect all y-values for a given row n.
   The values are stored in vals (assumed large enough) and their
   count is written to out_cnt.  They are sorted ascending and made
   unique. */
static void collect_row(idx_t n, seq_t *vals, size_t *out_cnt)
{
    idx_t S = 2 * n;          /* S = 2*n  */
    size_t cnt = 0;
    /* Temporary array of y-values; 256 is plenty for n ≤ 10000. */
    seq_t tmp[256];

    for (idx_t d = 1; d * d <= S; ++d) {
        if (S % d != 0) continue;
        idx_t d1 = d;
        idx_t d2 = S / d;

        /* Process divisor d1 if even */
        if ((d1 & 1U) == 0U) {
            /* even x case: x = d1 */
            seq_t y_even = (seq_t)(S / d1) + 1;
            if (y_even >= (seq_t)d1) {
                tmp[cnt++] = y_even;
            }
            /* odd x case: x = d1 + 1 (odd because d1 is even) */
            seq_t x_odd = (seq_t)d1 + 1;
            seq_t y_odd = (seq_t)(S / d1) - 1;
            if (y_odd >= x_odd) {
                tmp[cnt++] = y_odd;
            }
        }

        /* If d2 is distinct, process it */
        if (d2 != d1) {
            if ((d2 & 1U) == 0U) {
                seq_t y_even = (seq_t)(S / d2) + 1;
                if (y_even >= (seq_t)d2) {
                    tmp[cnt++] = y_even;
                }
                seq_t x_odd = (seq_t)d2 + 1;
                seq_t y_odd = (seq_t)(S / d2) - 1;
                if (y_odd >= x_odd) {
                    tmp[cnt++] = y_odd;
                }
            }
        }
    }

    /* Sort and deduplicate */
    if (cnt > 1) {
        qsort(tmp, cnt, sizeof(seq_t), seq_cmp);
        size_t uniq = 1;
        for (size_t i = 1; i < cnt; ++i) {
            if (tmp[i] != tmp[uniq - 1]) {
                tmp[uniq++] = tmp[i];
            }
        }
        cnt = uniq;
    }

    /* Copy to output buffer */
    for (size_t i = 0; i < cnt; ++i) {
        vals[i] = tmp[i];
    }
    *out_cnt = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1 – behaviour below offset is irrelevant */

    idx_t seen = 0;            /* number of terms already produced so far */
    idx_t row  = 1;            /* current row number */
    seq_t vals[256];
    size_t row_cnt;

    while (1) {
        collect_row(row, vals, &row_cnt);

        if (seen + row_cnt >= n) {
            /* The desired term is in this row */
            return vals[n - seen - 1];
        }

        seen += row_cnt;
        row++;
    }
}
