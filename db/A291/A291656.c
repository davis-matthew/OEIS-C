/*
 * A291656  Square array T(n,k), n>=0, k>=0, read by antidiagonals: T(n,k) =
 *          ((2n-1)!!)^k * Sum_{i=1..n} 1/(2*i-1)^k.
 * https://oeis.org/A291656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * T(0,k) = 0 * T(1,k) = 1 * T(n+1,k) = ((2n-1)^k + (2n+1)^k) *
 *            T(n,k) * - (2n-1)^(2k) * T(n-1,k) (n ≥ 1) * * The sequence a(n)
 *            is read by antidiagonals from T(n,k) (a(0) = 0)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 48, u64 n <= 71, u128 n <= 126,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                       /* for size_t          */
#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array */
#include "../utils/seq_free_array.h"      /* seq_free_array     */

static seq_t *T_table = NULL;   /* pointer to allocated 2‑D table          */
static idx_t  T_size  = 0;      /* number of rows/cols in T_table (max_n+1) */

/* Ensure that T_table contains rows/cols 0 … max_nd (inclusive).   */
static void ensure_table(idx_t max_nd)
{
    /* T_size is the number of rows already allocated.           */
    if (max_nd + 1 <= T_size)
        return;                      /* table already large enough */

    idx_t      new_size = max_nd + 1;                /* rows = cols    */
    size_t     table_cells = (size_t)new_size * new_size;
    seq_t     *new_table = seq_alloc_new_array(table_cells);
    if (!new_table)                      /* allocation failed   */
        exit(1);

    /* initialise all entries to zero */
    for (size_t i = 0; i < table_cells; ++i)
        new_table[i] = 0;

    /* base rows */
    for (idx_t k = 0; k < new_size; ++k) {
        new_table[0 * new_size + k] = 0;          /* T(0,k)=0      */
        if (new_size > 1)
            new_table[1 * new_size + k] = 1;      /* T(1,k)=1      */
    }

    /* recurrence for n >= 2 */
    for (idx_t n = 2; n < new_size; ++n) {
        seq_t base_a = (seq_t)(2 * (n - 1) - 1);   /* 2n-3            */
        seq_t base_b = (seq_t)(2 * (n - 1) + 1);   /* 2n-1            */
        seq_t base_a2 = base_a * base_a;          /* (2n-3)^2        */

        seq_t val_a   = 1;    /* a^k            */
        seq_t val_b   = 1;    /* b^k            */
        seq_t val_a2k = 1;    /* a^{2k}         */

        for (idx_t k = 0; k < new_size; ++k) {
            seq_t sum_ab = val_a + val_b;
            seq_t term1  = sum_ab * new_table[(n - 1) * new_size + k];
            seq_t term2  = val_a2k * new_table[(n - 2) * new_size + k];
            new_table[n * new_size + k] = term1 - term2;

            /* advance powers for next k */
            val_a   = val_a   * base_a;
            val_b   = val_b   * base_b;
            val_a2k = val_a2k * base_a2;
        }
    }

    /* discard old table (if any) */
    if (T_table)
        seq_free_array(T_table);

    T_table = new_table;
    T_size  = new_size;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) = 0 */

    /* locate the diagonal (antidiagonal index) containing n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t start  = d * (d + 1) / 2;
    idx_t offset = n - start;          /* position on this diagonal  */
    idx_t i = offset;                  /* row index on this diagonal */
    idx_t k = d - i;                   /* column index            */

    ensure_table(d);                    /* guarantee T(i,k) computed */

    return T_table[i * T_size + k];
}
