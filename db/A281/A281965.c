/*
 * A281965  Number of sets of exactly n positive integers <= n+2 having a
 *          square element sum.
 * https://oeis.org/A281965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of subsets of size n from {1..n+2} whose element
 *            sum is a perfect * square. Each such subset corresponds to a
 *            pair of omitted elements; * the sum of the omitted pair must be
 *            T - s², where T = (n+2)(n+3)/2 * and s is a non‑negative
 *            integer. The algorithm enumerates all * squares s² ≤...
 * Range:     u8 n <= 18, u16 n <= 469, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This runs in O(n) time with a small constant. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute total sum of all numbers 1..n+2 */
    seq_t n_plus_2 = (seq_t)(n + 2);
    seq_t n_plus_3 = (seq_t)(n + 3);
    seq_t T = (n_plus_2 * n_plus_3) / 2;

    /* Squares are considered up to T-3, because the minimal
     * possible sum of two distinct numbers is 3.                 */
    seq_t limit = T - 3;

    seq_t total = 0;

    for (idx_t s = 0; ; ++s) {
        seq_t sq = (seq_t)s * (seq_t)s;
        if (sq > limit) break;          /* no more squares to test */

        seq_t k = T - sq;               /* sum of omitted pair */

        /* k must lie between 3 and 2n+3 inclusive. */
        if (k < 3 || k > 2*(seq_t)n + 3) continue;

        /* Number of unordered pairs (i,j) with 1 ≤ i < j ≤ n+2
         * and i + j = k.                                     */
        seq_t min_i = (k > n_plus_3) ? (k - n_plus_2) : 1;
        seq_t max_i = (k - 1) / 2;      /* floor((k-1)/2) */

        if (max_i < min_i) continue;   /* no valid pairs */

        seq_t add = max_i - min_i + 1;
        total += add;
    }

    return total;
}
