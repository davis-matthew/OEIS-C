/*
 * A390443  Irregular triangle read by rows: T(n,k) is the sum of even numbers
 *          in the k-th 2-dense sublist of divisors of n, with n >= 1, k >= 1.
 * https://oeis.org/A390443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 266, u16 n <= 9470, u32 n <= 9470, u64 n <= 9470,
 *            u128 n <= 9470, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A390443.
   a(n) is obtained by concatenating, for each row k ≥ 1,
   the sums of even numbers in each maximal 2‑dense sublist
   of the sorted divisors of k.  The function scans rows
   until the desired index is reached. */
#include <stdlib.h>

/* Comparator for qsort */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *va = (const idx_t *)a;
    const idx_t *vb = (const idx_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* n is the 1‑based position in the flattened sequence. */
    idx_t target = n;
    idx_t row = 1;                   /* current row number (divisors of row) */

    while (1) {
        /* collect all divisors of `row` */
        idx_t divs[256];              /* 256 sufficient for divisors of small n */
        size_t dcnt = 0;
        for (idx_t d = 1; d * d <= row; ++d) {
            if (row % d == 0) {
                divs[dcnt++] = d;
                if (d != row / d) divs[dcnt++] = row / d;
            }
        }
        /* sort the divisors */
        qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

        /* iterate over contiguous 2‑dense sublists */
        seq_t sub_sum = 0;
        idx_t prev = 0;

        for (size_t i = 0; i < dcnt; ++i) {
            idx_t curr = divs[i];
            if (i == 0) {               /* start of first sublist */
                prev = curr;
                sub_sum = ((curr & 1) == 0) ? (seq_t)curr : (seq_t)0;
                continue;
            }
            if (curr > prev * 2) {        /* start of a new sublist */
                if (target == 1) return sub_sum;
                target--;
                sub_sum = 0;
            }
            if ((curr & 1) == 0) sub_sum += (seq_t)curr;
            prev = curr;
        }

        /* last sublist in this row */
        if (target == 1) return sub_sum;
        target--;
        ++row;                           /* move to next row */
    }
}
