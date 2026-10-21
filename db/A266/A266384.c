/*
 * A266384  Total number of OFF (white) cells after n iterations of the "Rule
 *          22" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 – (# of ON cells in rows 0..n when Rule 22 is run
 *            with a single ON cell in the middle). The function simulates
 *            Rule 22 up to the requested iteration, counts the ON cells, and
 *            subtracts that number from the total of (n+1)^2 cells in the
 *            triangular array
 * Range:     u8 n <= 18, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* maximum width needed: 2*n+1 cells */
    size_t N   = (size_t)n;
    size_t len = 2 * N + 1;

    /* allocate two working rows (cast for C++ compatibility) */
    unsigned char *cur  = (unsigned char*)malloc(len);
    unsigned char *next = (unsigned char*)malloc(len);
    if (!cur || !next) {
        /* allocation failed – return 0 (should not happen) */
        free(cur);
        free(next);
        return 0;
    }

    /* initialise all to 0 */
    memset(cur, 0, len);
    memset(next, 0, len);

    /* initial configuration: single ON cell at the centre */
    cur[N] = 1;
    seq_t total_on = 1;  /* ON cells counted so far (row 0) */

    for (size_t t = 1; t <= N; ++t) {
        size_t left  = N - t;
        size_t right = N + t;
        seq_t new_on = 0;

        for (size_t i = left; i <= right; ++i) {
            unsigned char l = (i > 0) ? cur[i - 1] : 0;
            unsigned char c = cur[i];
            unsigned char r = (i + 1 < len) ? cur[i + 1] : 0;
            unsigned char v = (l + c + r == 1) ? 1 : 0;
            next[i] = v;
            if (v) ++new_on;
        }
        total_on += new_on;

        /* swap rows for next iteration */
        unsigned char *tmp = cur;
        cur = next;
        next = tmp;
    }

    free(cur);
    free(next);

    /* (n+1)^2 cells in the triangular array up to row n */
    seq_t cells  = (seq_t)(n + 1) * (seq_t)(n + 1);
    seq_t result = cells - total_on;
    return result;
}
