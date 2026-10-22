/*
 * A378200  Square array read by upward antidiagonals: T(n,k) = ((k + n - 1)^2
 *          + (k - n + 1)*(-1)^n + (1 - k - n)*(-1)^k + (1 - k - n)*(-1)^(k +
 *          n) + 2)/2.
 * https://oeis.org/A378200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite array T(n,k) = ((k + n
 *            - 1)^2 + (k - n + 1)*(-1)^n + (1 - k - n)*(-1)^k + (1 - k -
 *            n)*(-1)^(k + n) + 2)/2 along antidiagonals with constant n+k
 * Range:     u8 n <= 233, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   T_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/T_array.h"

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;   /* undefined for non‑positive indices */

    /* Find the diagonal sum s (= n + k) that contains the nth term. */
    idx_t s   = 2;     /* first diagonal has sum 2 (term (1,1)) */
    idx_t cum = 0;     /* count of terms before diagonal s */

    while (cum + (s - 1) < n) {
        cum += (s - 1); /* add number of terms in this diagonal */
        s   += 1;
    }

    idx_t offset = n - cum - 1;           /* 0‑based position in diagonal */
    idx_t row    = s - 1 - offset;        /* n in the array */
    idx_t col    = s - row;               /* k in the array */

    return T_array(row, col);
}
