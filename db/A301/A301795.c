/*
 * A301795  Number of 6 X n 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3) for n > 6,
   initial terms: a(1)=32, a(2)=233, a(3)=96, a(4)=116, a(5)=150, a(6)=205 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *tbl = NULL;
    static idx_t tbl_sz = 0;

    /* initialize base values at first call */
    if (tbl == NULL) {
        tbl_sz = 7;                     /* indices 0..6 */
        tbl = (seq_t *)malloc(tbl_sz * sizeof(seq_t));
        if (!tbl) abort();
        memset(tbl, 0, tbl_sz * sizeof(seq_t));
        tbl[1] = 32;
        tbl[2] = 233;
        tbl[3] = 96;
        tbl[4] = 116;
        tbl[5] = 150;
        tbl[6] = 205;
    }

    /* already computed? */
    if (n < tbl_sz && tbl[n] != 0)
        return tbl[n];

    /* n <= 6 handled by base table above */
    if (n < 7)
        return tbl[n];

    /* extend table if necessary */
    if (n >= tbl_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(tbl, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        tbl = tmp;
        memset(tbl + tbl_sz, 0, (new_sz - tbl_sz) * sizeof(seq_t));
        tbl_sz = new_sz;
    }

    /* compute missing entries */
    for (idx_t i = 7; i <= n; ++i) {
        if (tbl[i] != 0) continue;          /* may already be computed */
        tbl[i] = (seq_t)2 * tbl[i - 1] - tbl[i - 3];
    }

    return tbl[n];
}
