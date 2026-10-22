/*
 * A336633  Triangle read by rows: T(n,k) is the number of generalized
 *          permutations related to the degenerate Eulerian numbers with
 *          exactly k ascents (0 <= k <= max(0,n-1)).
 * https://oeis.org/A336633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:  T(n,k) = (n+k)T(n-1,k)+(2n-k-1)T(n-1,k-1)  (T(0,0)=1)  */
/* a(n) is the flattened sequence of the triangle.  */

#include <stdlib.h>

static seq_t **triangle = NULL;      /* array of row pointers */
static size_t triangle_cap = 0;      /* number of rows allocated */

/* Resize the triangle array to hold at least new_cap rows. */
static void ensure_triangle_cap(size_t new_cap)
{
    if (triangle_cap >= new_cap)
        return;
    seq_t **tmp = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
    for (size_t i = triangle_cap; i < new_cap; ++i)
        tmp[i] = NULL;
    triangle = tmp;
    triangle_cap = new_cap;
}

/* Allocate a row containing sz elements.  */
static seq_t *alloc_row(size_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t*)malloc(sz * sizeof(seq_t));
#endif
}

/* Return the n-th term of A336633.  */
seq_t a(idx_t n)
{
    /* Find the row r and column k that correspond to index n. */
    idx_t cumulative = 0;
    size_t r = 0;
    for (;; ++r) {
        size_t len = (r == 0 ? 1 : r);           /* length of row r */
        if (cumulative + (idx_t)len > n)
            break;
        cumulative += (idx_t)len;
    }
    size_t k = (size_t)(n - cumulative);         /* column in row r */

    /* Ensure rows 0..r are allocated and computed. */
    ensure_triangle_cap(r + 1);
    for (size_t i = 0; i <= r; ++i) {
        if (triangle[i] != NULL)
            continue;   /* already computed */

        size_t sz = (i == 0 ? 1 : i);            /* length of row i */
        seq_t *row = alloc_row(sz);

        if (i == 0) {                   /* T(0,0) = 1 */
            row[0] = (seq_t)1;
        } else if (i == 1) {            /* T(1,0) = 1 */
            row[0] = (seq_t)1;
        } else {
            seq_t *prev = triangle[i - 1];
            for (size_t j = 0; j < sz; ++j) {   /* compute T(i,j) */
                seq_t term1 = 0;
                /* T(i-1,j) exists only when j < i-1 */
                if (j < i - 1)
                    term1 = ((seq_t)(i + j)) * prev[j];

                seq_t term2 = 0;
                /* T(i-1,j-1) exists only when j > 0 */
                if (j > 0)
                    term2 = ((seq_t)(2 * i - j - 1)) * prev[j - 1];

                row[j] = term1 + term2;
            }
        }
        triangle[i] = row;
    }
    return triangle[r][k];
}
