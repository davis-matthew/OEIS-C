/*
 * A318686  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1,k) -
 *          2*T(n-1,k-2) + T(n-1,k-3) for k = 0..3n; T(n,k)=0 for n or k < 0.
 * https://oeis.org/A318686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 43, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0,0)=1; T(n,k)=T(n-1,k)-2*T(n-1,k-2)+T(n-1,k-3) for 0≤k≤3n.
   a(n) is the n‑th element of the concatenation of the rows of T. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the row r such that the flat index n belongs to row r. */
    idx_t r = 0;
    idx_t cum = 0;
    while (1) {
        idx_t len = 3 * r + 1;          /* size of row r */
        if (n < cum + len) break;
        cum += len;                     /* cumulative size up to this row */
        ++r;
    }

    /* Allocate the first row (row 0). */
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t));
    prev[0] = 1;                         /* T(0,0) = 1 */

    /* If we need only row 0, return the result. */
    if (r == 0) {
        seq_t ans = prev[0];
        free(prev);
        return ans;
    }

    /* Iteratively build subsequent rows up to row r. */
    for (idx_t row = 1; row <= r; ++row) {
        idx_t prevLen = 3 * (row - 1) + 1;
        idx_t len     = 3 * row + 1;
        seq_t *curr = (seq_t*)malloc(sizeof(seq_t) * len);

        for (idx_t k = 0; k < len; ++k) {
            seq_t a1 = 0, a2 = 0, a3 = 0;
            if (k < prevLen)                 a1 = prev[k];
            if (k >= 2 && (k - 2) < prevLen) a2 = prev[k - 2];
            if (k >= 3 && (k - 3) < prevLen) a3 = prev[k - 3];
            curr[k] = a1 - 2 * a2 + a3;      /* recurrence */
        }
        free(prev);
        prev = curr;
    }

    /* Extract the element at the flat index n. */
    idx_t offset = n - cum;
    seq_t result = prev[offset];
    free(prev);
    return result;
}
