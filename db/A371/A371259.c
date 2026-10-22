/*
 * A371259  Triangle read by rows, (2, 3)-Lah numbers.
 * https://oeis.org/A371259
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 23, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: (2,3)-Lah numbers T(n,k)
 * Recurrence: T(n,k) = T(n-1,k-1) + (n+k-1)^2 * T(n-1,k)
 * with T(n,n)=1 and T(n,k)=0 for k<3 or n<k.
 * a(n) is the triangle read row‑wise starting at n = 3.  */
#include <stddef.h>
#include <stdlib.h>

static seq_t **table = NULL;    /* table[r] -> array of T(r,k), k = 0 … r   */
static size_t table_cap      = 0; /* capacity of the table array            */
static size_t max_row_computed = 2; /* highest row that has been calculated */

static void ensure_row(size_t r)
{
    if (r <= max_row_computed)
        return;

    /* enlarge the table array if needed */
    if (r + 1 > table_cap) {
        size_t new_cap = r + 1;
#ifdef __cplusplus
        table = (seq_t**)realloc(table, new_cap * sizeof(seq_t*));
#else
        table = realloc(table, new_cap * sizeof(seq_t*));
#endif
        table_cap = new_cap;
    }

    for (size_t cur = max_row_computed + 1; cur <= r; ++cur) {
        /* allocate space for row 'cur' (indices 0 … cur) */
        seq_t *row;
#ifdef __cplusplus
        row = (seq_t*)malloc((cur + 1) * sizeof(seq_t));
#else
        row = malloc((cur + 1) * sizeof(seq_t));
#endif
        /* initialise all entries to 0 */
        for (size_t i = 0; i <= cur; ++i)
            row[i] = 0;

        /* base case */
        row[cur] = 1;

        /* compute values for 3 ≤ k < cur */
        for (size_t k = 3; k < cur; ++k) {
            seq_t term1 = 0;
            if (k - 1 >= 3)
                term1 = table[cur - 1][k - 1];

            seq_t factor = (seq_t)(cur + k - 1);
            factor *= factor;

            seq_t term2 = factor * table[cur - 1][k];
            row[k] = term1 + term2;
        }

        table[cur] = row;
    }
    max_row_computed = r;
}

seq_t a(idx_t n)
{
    /* sequence starts at n = 3; behaviour below the offset is irrelevant */
    if (n < 3)
        return 0;

    /* convert n to a zero‑based position inside the triangle */
    size_t idx = (size_t)(n - 2);   /* 1‑based index into the triangle */

    /* find the row m such that cumulative count >= idx */
    size_t m = 3;
    while (((m - 2) * (m - 1)) / 2 < idx)
        ++m;

    /* number of elements before row m */
    size_t before = ((m - 3) * (m - 2)) / 2;

    /* offset within row m (zero‑based) */
    size_t offset = idx - before - 1;
    size_t k = offset + 3;   /* columns start at 3 */

    ensure_row(m);
    return table[m][k];
}
