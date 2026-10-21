/*
 * A242314  Largest digital root in row n of Pascal's triangle.
 * https://oeis.org/A242314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max_{0<=k<=n} digital_root( binom(n,k) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *row = NULL;
static size_t row_cap = 0;
static seq_t *res = NULL;
static size_t res_cap = 0;
static idx_t max_n = 0;

/* Ensure that array 'arr' has at least 'needed' elements. */
static void ensure_cap(seq_t **arr, size_t *cap, idx_t needed)
{
    if ((size_t)needed <= *cap) return;
    size_t newcap = *cap ? *cap * 2 : 1;
    if (newcap < (size_t)needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!row) {
        ensure_cap(&row, &row_cap, 1);
        ensure_cap(&res, &res_cap, 1);
        row[0] = (seq_t)1;      /* row 0: only 1 */
        res[0] = (seq_t)1;      /* a(0) = 1 */
        max_n = 0;
    }

    if (n <= max_n) return res[n];

    /* Build rows up to n */
    for (idx_t step = max_n + 1; step <= n; ++step) {
        ensure_cap(&row, &row_cap, step + 1);
        ensure_cap(&res, &res_cap, step + 1);

        row[step] = (seq_t)1;          /* edge coefficient */
        seq_t max_dr = 1;              /* current max digital root */

        /* compute interior coefficients right‑to‑left */
        for (idx_t k = step - 1; k > 0; --k) {
            seq_t tmp = row[k] + row[k - 1];
            if (tmp >= 9) tmp -= 9;           /* modulo 9 */
            row[k] = tmp;
            seq_t dr = (tmp == 0) ? (seq_t)9 : tmp;
            if (dr > max_dr) max_dr = dr;
        }

        res[step] = max_dr;
        max_n = step;
    }

    return res[n];
}
