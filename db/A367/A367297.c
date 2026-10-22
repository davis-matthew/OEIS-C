/*
 * A367297  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 2 + 3*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - 2*x - x^2.
 * https://oeis.org/A367297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangular array T(n,k) defined
 *            by: T(1,1) = 1; T(2,1) = 2, T(2,2) = 3; for n ≥ 3, T(n,k) =
 *            2·T(n-1,k) + 3·T(n-1,k-1) + T(n-2,k) – 2·T(n-2,k-1) –
 *            T(n-2,k-2), where terms with out‑of‑range indices are treated as
 *            0. The array is read row by row to form the flat sequenc...
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the offset (a(1) is the first term). */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Compute the index within row r. */
    idx_t before = (r - 1) * r / 2;           /* elements before row r */
    idx_t offset = n - before - 1;            /* zero‑based position in row r */

    /* Direct answers for the first two rows. */
    if (r == 1) return (seq_t)1;
    if (r == 2) return (offset == 0) ? (seq_t)2 : (seq_t)3;

    /* Allocate the first two rows. */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t) * 1);   /* row 1 */
    seq_t *prev1 = (seq_t *)malloc(sizeof(seq_t) * 2);   /* row 2 */
    prev2[0] = (seq_t)1;
    prev1[0] = (seq_t)2;
    prev1[1] = (seq_t)3;

    /* Build rows 3 … r. */
    seq_t *cur = NULL;
    for (idx_t i = 3; i <= r; ++i) {
        cur = (seq_t *)malloc(sizeof(seq_t) * i);
        for (idx_t k = 0; k < i; ++k) {
            seq_t pos = 0, sub = 0;

            /* Positive contributions. */
            if (k < i - 1)          pos += (seq_t)2 * prev1[k];
            if (k > 0)              pos += (seq_t)3 * prev1[k - 1];
            if (k < i - 2)          pos += prev2[k];

            /* Subtractive contributions. */
            if (k > 0 && k < i - 1) sub += (seq_t)2 * prev2[k - 1];
            if (k > 1)              sub += prev2[k - 2];

            cur[k] = pos - sub;
        }

        /* Advance the rolling array pointers. */
        free(prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    seq_t result = prev1[offset];
    free(prev2);
    free(prev1);
    return result;
}
