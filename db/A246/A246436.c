/*
 * A246436  Number of numbers 1,...,n not occurring in the Collatz trajectory
 *          starting with n.
 * https://oeis.org/A246436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers 1 ≤ k ≤ n that do NOT appear in the
 *            Collatz trajectory starting at n. The trajectory is the usual
 *            even/odd rule until reaching 1
 * Range:     u8 n <= 26, u16 n <= 2842, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* allocate a tiny visited array for indices 1..n */
    uint8_t *visited = NULL;
    if (n == 0) {
        /* for n = 0 (outside the offset) return 0 */
        return (seq_t)0;
    }
    visited = (uint8_t *)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!visited) return (seq_t)0;          /* memory failure -> 0 */

    seq_t cur = (seq_t)n;
    /* mark 1 at the end explicitly */
    while (cur != (seq_t)1) {
        if (cur <= (seq_t)n) {
            visited[(size_t)cur] = 1;
        }
        /* Collatz step */
        if ((cur & (seq_t)1) == 0) {
            cur = cur / (seq_t)2;
        } else {
            cur = (seq_t)3 * cur + (seq_t)1;
        }
    }
    visited[1] = 1;            /* 1 terminates the trajectory */

    /* count unvisited numbers */
    idx_t i;
    seq_t count = (seq_t)0;
    for (i = (idx_t)1; i <= n; ++i) {
        if (!visited[(size_t)i]) {
            count += (seq_t)1;
        }
    }

    free(visited);
    return count;
}
