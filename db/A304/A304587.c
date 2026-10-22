/*
 * A304587  A linear mapping a(n) = x + d*n of pairs of integers (x,d), where
 *          the pairs are enumerated by a number spiral along antidiagonals.
 * https://oeis.org/A304587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = x + d*n where the pair (x,d) is obtained by enumerating
 *            all integer pairs (x,d) in a number spiral along antidiagonals.
 *            The diagonal containing n has index k and consists of the 4k
 *            pairs enumerated in the following order:
 *            (k,0),(k-1,1),…,(0,k),(-1,k-1),…,-(k,0),(-k+1,-(k-1)),…,(k-1,-(k-1))...
 * Range:     i8 n <= 36, i16 n <= 1325, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Find the diagonal k containing index n.
       sum = total number of terms up to and including diagonal k. */
    idx_t k = 0;
    idx_t sum = 1;          /* diagonal 0 contributes 1 term */
    while (n >= sum) {
        k++;
        sum += 4 * k;       /* diagonal k adds 4k terms */
    }

    /* total number of terms before diagonal k */
    idx_t count_before = (k == 0) ? 0 : (1 + 2 * (k - 1) * k);
    idx_t r = n - count_before;      /* 0‑based position inside the diagonal */

    seq_t x, d;

    /* Compute the pair (x,d) from the position r. */
    if (r <= k) {                     /* first part of the diagonal */
        x = (seq_t)k - (seq_t)r;
        d = (seq_t)r;
    }
    else if (r <= 2 * k) {           /* second part */
        idx_t offset2 = r - (k + 1);
        x = -((seq_t)offset2 + 1);
        d = (seq_t)k - (seq_t)offset2 - 1;
    }
    else {                           /* third part */
        idx_t offset3 = r - (2 * k + 1);
        seq_t temp = -((seq_t)k) + 1 + (seq_t)offset3; /* signed x */
        x = temp;
        seq_t absx = temp >= 0 ? temp : -temp;
        d = -((seq_t)k - absx);
    }

    /* Return the term a(n) = x + d*n. */
    return x + d * (seq_t)n;
}
