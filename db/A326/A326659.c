/*
 * A326659  T(n,k) = [0<k<=n] * n*(T(n-1,k-1)+T(n-1,k)) + [k=0 and n>=0];
 *          triangle T(n,k), n >= 0, 0 <= k <= n, read by rows.
 * https://oeis.org/A326659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 69, u64 n <= 175,
 *            u128 n <= 446, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = T(r,k)  where r = floor( (sqrt(1+8n)-1)/2 )
 *   k = n - r*(r+1)/2
 *   T(n,k) is defined by
 *     T(0,0) = 1
 *     T(n,0) = 1 for n>0
 *     T(n,k) = n * ( T(n-1,k-1) + T(n-1,k) )   for 0<k≤n
 *   Thus a(n) is the (r,k) entry of the triangle read by rows.
 *
 * The implementation keeps all previously computed rows in a static
 * array so that subsequent calls with different (possibly decreasing)
 * n values are fast.  The maximal row needed for n≤10000 is only
 * 141, so the storage is tiny even for arbitrary precision integers.
 */

#include <stdlib.h>

/* static cache of rows; rows[i] points to an array of size i+1 */
static seq_t **rows = NULL;
static size_t rows_len = 0;   /* number of rows currently built (max index+1) */

seq_t a(idx_t n)
{
    /* --- find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 --- */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* --- column index k --- */
    idx_t k = n - r * (r + 1) / 2;

    /* --- ensure rows up to r are built --- */
    while (rows_len <= (size_t)r) {
        /* allocate a new row of length rows_len+1 */
        seq_t *new_row = (seq_t *)malloc(sizeof(seq_t) * (rows_len + 1));
        if (!new_row) {
            /* out of memory – abort (unlikely for the given limits) */
            abort();
        }

        /* initialize the new row */
        new_row[0] = (seq_t)1;                /* T(rows_len,0) = 1 */
        if (rows_len > 0) {
            /* use previous row to compute values for j = 1..rows_len-1 */
            seq_t *prev = rows[rows_len - 1];
            for (size_t j = 1; j < (size_t)rows_len; ++j) {
                new_row[j] = (seq_t)rows_len * (prev[j - 1] + prev[j]);
            }
            /* last element */
            new_row[rows_len] = (seq_t)rows_len * prev[rows_len - 1];
        }

        /* store the row */
        if (!rows) {
            rows = (seq_t **)malloc(sizeof(seq_t *));
            if (!rows) abort();
            rows[0] = new_row;
        } else {
            seq_t **tmp = (seq_t **)realloc(rows, sizeof(seq_t *) * (rows_len + 1));
            if (!tmp) abort();
            rows = tmp;
            rows[rows_len] = new_row;
        }
        ++rows_len;
    }

    /* --- return the requested entry --- */
    return rows[r][(size_t)k];
}
