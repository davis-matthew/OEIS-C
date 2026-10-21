/*
 * A290456  Triangle formed by reading the triangle of Eulerian numbers
 *          (A173018) mod 6.
 * https://oeis.org/A290456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Eulerian number A(r,c) modulo 6, where r =
 *            floor((sqrt(8n+1)-1)/2) and c = n - r(r+1)/2. (Eulerian triangle
 *            rows are of length r+1 with A(r,r)=0 for r>0.)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t
find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Exponential search for an upper bound */
    while (((unsigned __int128)hi * (hi + 1) / 2) <= n) {
        hi <<= 1;
    }
    /* Binary search on the range [lo, hi) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo - 1;                /* r satisfies r(r+1)/2 <= n */
}

seq_t a(idx_t n)
{
    /* Special case for the first element */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Determine row r and column c */
    idx_t r = find_row(n);
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t c = n - start;

    /* For row 0, a(0) is already handled */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate two rows of size r+1 (but we use only the previous row of size r) */
    size_t size_r   = (size_t)r + 1;
    seq_t *prev = (seq_t*)malloc(size_r * sizeof(seq_t));
    seq_t *curr = (seq_t*)malloc(size_r * sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failure – return 0 as a safe fallback */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Row 0 */
    prev[0] = (seq_t)1;

    /* Compute rows up to r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)1;                     /* A(i,0) = 1 */
        for (idx_t k = 1; k < i; ++k) {
            seq_t term1 = ((seq_t)(i - k) * prev[k - 1]) % (seq_t)6;
            seq_t term2 = ((seq_t)(k + 1) * prev[k])      % (seq_t)6;
            curr[k] = (term1 + term2) % (seq_t)6;
        }
        curr[i] = (seq_t)0;                     /* A(i,i) = 0 for i>0 */

        /* Swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c] % (seq_t)6;           /* modulo 6 already maintained */

    /* Free the temporary storage */
    free(prev);
    free(curr);

    return result;
}
