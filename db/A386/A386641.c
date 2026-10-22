/*
 * A386641  Triangle read by row: T(n,k) is the number of the k-th eliminated
 *          person in a variant of the Josephus problem in which one person is
 *          skipped, then one is eliminated, then two people are skipped, then
 *          the next person is eliminated and so on.
 * https://oeis.org/A386641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the (k)-th eliminated person in the Josephus variant
 *            where the skip length starts at 1 and increases by 1 after each
 *            elimination, read row‑by‑row from the triangular array T(n,k)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the value a(n) defined by the OEIS entry A386641.  */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.  */
    idx_t r = 1;
    for (;; ++r) {
        idx_t tri = r * (r + 1) / 2;
        if (tri >= n)
            break;
    }

    /* Column index k within that row: k starts from 1. */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* Allocate circular doubly linked list arrays.  */
    idx_t *next = (idx_t *)malloc((r + 1) * sizeof(idx_t));
    idx_t *prev = (idx_t *)malloc((r + 1) * sizeof(idx_t));
    if (!next || !prev) {
        /* Memory allocation failed; return 0 as a safe fall‑back. */
        free(next);
        free(prev);
        return (seq_t)0;
    }

    /* Initialize the list with 1..r. */
    for (idx_t i = 1; i <= r; ++i) {
        next[i] = (i == r) ? 1 : i + 1;
        prev[i] = (i == 1) ? r : i - 1;
    }

    /* Simulate eliminations until the k‑th one.  */
    idx_t cur = 1;          /* current position index */
    idx_t elimination_value = 1;
    for (idx_t step = 1; step <= k; ++step) {
        /* Skip 'step' persons, eliminating the next.  */
        idx_t pos = cur;
        for (idx_t t = 0; t < step; ++t)
            pos = next[pos];            /* move forward step times */

        elimination_value = pos;       /* person eliminated this step */

        if (step == k) {
            /* The desired entry; clean up and return. */
            free(next);
            free(prev);
            return (seq_t)elimination_value;
        }

        /* Remove 'pos' from the circular list.  */
        idx_t left = prev[pos];
        idx_t right = next[pos];
        next[left] = right;
        prev[right] = left;

        /* Move current to the next alive person after the removed one. */
        cur = right;
    }

    /* Should never reach here.  */
    free(next);
    free(prev);
    return (seq_t)0;
}
