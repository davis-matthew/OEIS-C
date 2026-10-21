/*
 * A269955  Triangle read by rows, T(n, k) = S(k, n) with S(n, n) = 1, S(0, n)
 *          = 0 and otherwise S(k,n) = Sum_{i=1..n-k+1} k^i*S(k-1, n-i), for
 *          n>=0 and 0<=k<=n.
 * https://oeis.org/A269955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element k of row r, where r is the largest integer *
 *            satisfying r*(r+1)/2 ≤ n. Then k = n − r*(r+1)/2. * * The
 *            triangle T(n,k) = S(k,n) is defined by * * S(n,n) = 1, * S(0,n)
 *            = 0 (for n>0, S(0,0)=1 is the first entry of the triangle), *
 *            S(k,n) = Σ_{i=1}^{n−k+1} k^i · S(k−1, n−i) (for 0<k<n...
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_idx.h"

/* triangular storage of S(k,n)   (index = _idx(n,k)) */
static seq_t *S   = NULL;   /* array of computed values */
static idx_t rows_computed = 0;   /* number of rows already filled */

/* ensure that rows 0 .. max_r are computed */
static void _ensure_rows(idx_t max_r)
{
    if (max_r < rows_computed) return;   /* already ready */

    /* compute new capacity as triangular number for max_r */
    idx_t new_cap = (max_r + 1) * (max_r + 2) / 2;

    /* allocate or grow the array */
    if (S == NULL) {
        S = (seq_t*)malloc(sizeof(seq_t) * new_cap);
    } else {
        S = (seq_t*)realloc(S, sizeof(seq_t) * new_cap);
    }

    /* compute the new rows one by one */
    for (idx_t r = rows_computed; r <= max_r; ++r) {
        for (idx_t k = 0; k <= r; ++k) {
            seq_t val;
            if (r == 0 && k == 0) {
                /* S(0,0) = 1 */
                val = (seq_t)1;
            } else if (k == 0) {
                /* S(0,n) = 0 for n>0 */
                val = (seq_t)0;
            } else if (k == r) {
                /* S(n,n) = 1 */
                val = (seq_t)1;
            } else {
                /* Recurrence: S(k,n) = Σ_{i=1}^{n-k+1} k^i · S(k-1, n-i) */
                seq_t sum = (seq_t)0;
                seq_t power = (seq_t)k;   /* k^1 initially */
                for (idx_t i = 1; i <= r - k + 1; ++i) {
                    /* multiply by k to get k^i for next iteration */
                    if (i > 1) power *= (seq_t)k;
                    /* add term: k^i * S(k-1, r-i) */
                    sum += power * S[_idx(r - i, k - 1)];
                }
                val = sum;
            }
            S[_idx(r, k)] = val;
        }
    }

    rows_computed = max_r + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t k = n - r * (r + 1) / 2;

    _ensure_rows(r);
    return S[_idx(r, k)];
}
