/*
 * A372208  Triangle read by rows, (3, 3)-Lah numbers.
 * https://oeis.org/A372208
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑3)‑th element of the triangle read by rows * of
 *            the (3,3)-Lah numbers T(r,k), with r≥3, k in [3,r] and * T(r,k)
 *            = T(r‑1,k‑1) + (r+k‑1)^3 · T(r‑1,k), * T(r,r)=1, T(r,k)=0 for
 *            k<3 or r<k
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 12, u64 n <= 30, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The function is memoized and works for any order of calls. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build.  Some builds may use
 * arbitrary‑precision integer types; any arithmetic defined for that
 * type is available.  No additional headers are required.  */

static seq_t **lap_rows = NULL;      /* triangular array of rows */
static size_t  max_row  = 2;          /* largest row computed so far */

/* Ensure that rows up to 'up_to' have been computed and cached. */
static void
ensure_rows(idx_t up_to)
{
    if (up_to <= max_row)
        return;

    /* Expand the array of row pointers. */
    size_t new_size = (size_t)up_to + 1;     /* indices up to up_to inclusive */
    lap_rows = (seq_t **)realloc(lap_rows, new_size * sizeof(seq_t *));
    if (!lap_rows)
        abort();                          /* out of memory */

    /* Compute rows one by one. */
    for (size_t r = max_row + 1; r <= (size_t)up_to; ++r) {
        size_t len = r - 2;                   /* number of entries in this row */
        seq_t *row = (seq_t *)calloc(len, sizeof(seq_t));
        if (!row)
            abort();

        if (r == 3) {                         /* base case T(3,3) = 1 */
            row[0] = 1;
        } else {
            seq_t *prev = lap_rows[r - 1];     /* previous row */
            for (idx_t k = 3; k <= r; ++k) {
                seq_t val = 0;

                /* Term (r + k - 1)^3 * T(r-1,k) */
                if (k <= r - 1) {
                    seq_t m  = (seq_t)(r + k - 1);
                    seq_t m3 = m * m * m;
                    val += m3 * prev[k - 3];
                }

                /* Term T(r-1,k-1) */
                if (k - 1 >= 3)
                    val += prev[k - 4];

                row[k - 3] = val;
            }
        }

        lap_rows[r] = row;
        max_row = r;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                    /* behaviour below offset irrelevant */

    /* Find the row r such that (r-2)(r-1)/2 > i where i = n-3. */
    idx_t i   = n - 3;
    idx_t r   = 3;
    while ( ((idx_t)(r - 2) * (r - 1) / 2) <= i )
        ++r;

    /* Position of the element inside row r. */
    idx_t prev_tri = (idx_t)(r - 3) * (r - 2) / 2;
    idx_t pos      = i - prev_tri;       /* 0‑based offset in the row */
    idx_t k        = 3 + pos;            /* column index */

    ensure_rows(r);
    return lap_rows[r][k - 3];
}
