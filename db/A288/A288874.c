/*
 * A288874  Row reversed version of triangle A201637 (second-order Eulerian
 *          triangle).
 * https://oeis.org/A288874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 69, u64 n <= 175,
 *            u128 n <= 471, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r, m) where
 *   r = the unique integer such that r*(r+1)/2 <= n < (r+1)*(r+2)/2,
 *   m = n - r*(r+1)/2.
 *
 * The table T is the “row‐reversed” second–order Eulerian triangle,
 * defined by
 *
 *   T(0,0) = 1,
 *   T(n,0) = 0   for n > 0,
 *   T(n,n) = 1,  for n > 0,
 *   T(n,m) = (n-m+1)*T(n-1,m-1) + (n-1+m)*T(n-1,m)   for 0 < m < n.
 *
 * The function builds the table lazily and caches all rows up to the
 * required index, so subsequent calls are O(1).  The implementation
 * uses only integer arithmetic and works for all allowed `seq_t`
 * implementations.
 */

#include <stdlib.h>
#include <stdint.h>

/* external integer and value types */
typedef uint64_t idx_t;   /* as defined in the build system */

static seq_t **T = NULL;       /* array of pointers to rows of T   */
static size_t rows_alloced = 0; /* number of rows currently stored */

/* Helper: compute row index r for a given overall index n */
static inline idx_t row_for_idx(idx_t n)
{
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) {
        ++r;
    }
    return r - 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine which row and which column of T we need */
    idx_t r = row_for_idx(n);
    idx_t start = r * (r + 1) / 2;
    idx_t m = n - start;

    /* Lazy allocation of rows up to r */
    if (rows_alloced <= (size_t)r) {
        /* Grow the array of row pointers if needed */
        size_t new_rows = r + 1;
        if (T == NULL) {
            T = (seq_t **)malloc(new_rows * sizeof(seq_t *));
        } else {
            T = (seq_t **)realloc(T, new_rows * sizeof(seq_t *));
        }
        /* Build new rows one by one */
        for (size_t i = rows_alloced; i < new_rows; ++i) {
            T[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            for (size_t j = 0; j <= i; ++j) {
                if (i == 0) {           /* base case */
                    T[i][j] = (seq_t)1;
                } else if (j == 0) {    /* first column for n>0 is zero */
                    T[i][j] = (seq_t)0;
                } else if (j == i) {    /* diagonal */
                    T[i][j] = T[i - 1][j - 1];
                } else {                /* general recurrence */
                    seq_t term1 = ((seq_t)(i - j + 1)) * T[i - 1][j - 1];
                    seq_t term2 = ((seq_t)(i - 1 + j)) * T[i - 1][j];
                    T[i][j] = term1 + term2;
                }
            }
        }
        rows_alloced = new_rows;
    }

    return T[r][m];
}
