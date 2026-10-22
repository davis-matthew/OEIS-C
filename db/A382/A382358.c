/*
 * A382358  Triangle read by rows: T(n,k) is the number of the k-th eliminated
 *          person in the variation of the Josephus elimination process for n
 *          people, where in each round, the first person is skipped, the
 *          second eliminated and the third is skipped.
 * https://oeis.org/A382358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   a(n) is the n-th term of triangle T, where T(r,k) is the k-th
   eliminated person in the Josephus variant that skips one person,
   eliminates the next, then skips one.  The flattened sequence is
   read row by row (row 1 contains 1 element, row 2 contains 2, …).
   For a given index n, find the row r such that r(r+1)/2 ≥ n,
   then determine k = n – r(r-1)/2, and simulate the process for
   r people until the k-th elimination.  The eliminated number is
   returned as a(n). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find row r (smallest r with r*(r+1)/2 >= n) */
    idx_t r = 0;
    idx_t tot = 0;
    while (tot < n) {
        ++r;
        tot += r;
    }
    idx_t prev_tot = tot - r;          /* T(r-1) */
    idx_t k = n - prev_tot;            /* position in row r (1‑based) */

    /* Allocate circular linked list for r elements */
    idx_t *next = (idx_t*)malloc((r + 1) * sizeof(idx_t));
    idx_t *prev = (idx_t*)malloc((r + 1) * sizeof(idx_t));
    if (!next || !prev) {              /* allocation failure; return 0 */
        if (next) free(next);
        if (prev) free(prev);
        return (seq_t)0;
    }

    for (idx_t i = 1; i <= r; ++i) {
        next[i] = (i == r) ? 1 : i + 1;
        prev[i] = (i == 1) ? r : i - 1;
    }

    idx_t cur = 1;
    seq_t result = 0;

    for (idx_t step = 1; step <= k; ++step) {
        idx_t elim = next[cur];
        if (step == k) {
            result = (seq_t)elim;
            break;
        }
        /* Unlink elim */
        next[prev[elim]] = next[elim];
        prev[next[elim]] = prev[elim];
        /* Skip one after elimination */
        cur = next[next[elim]];
    }

    free(next);
    free(prev);
    return result;
}
