/*
 * A251397  Number of (n+1)X(3+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A251397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A251397 counts (n+1)×4 arrays over {0,1,2,3} with the
 * property that for every 2×2 subblock
 *
 *     a b
 *     c d
 *
 * the inequality a + d ≥ |b − c| holds.
 * This DP algorithm iterates over all row states (encoded as 4 two-bit
 * numbers, 256 possible rows).  A transition matrix records which
 * rows may follow which ones.  The number of admissible arrays of
 * height n+1 is obtained by summing the counts after n transitions.
 *
 * The first 11 terms match the OEIS data.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool allowed[256][256];
static bool allowed_built = false;

static void build_allowed(void)
{
    for (int r = 0; r < 256; ++r) {
        for (int s = 0; s < 256; ++s) {
            bool ok = true;
            for (int j = 0; j < 3; ++j) {
                int a = (r >> (2*j)) & 3;
                int b = (r >> (2*(j+1))) & 3;
                int c = (s >> (2*j)) & 3;
                int d = (s >> (2*(j+1))) & 3;
                if ((a + d) < abs(b - c)) { ok = false; break; }
            }
            allowed[r][s] = ok;
        }
    }
    allowed_built = true;
}

seq_t a(idx_t n)
{
    if (!allowed_built) build_allowed();

    /* Allocate DP arrays */
    seq_t *cur;
    seq_t *next;
#ifdef __cplusplus
    cur  = new seq_t[256]();
    next = new seq_t[256]();
#else
    cur  = (seq_t*)calloc(256, sizeof(seq_t));
    next = (seq_t*)calloc(256, sizeof(seq_t));
#endif

    /* Every first row is allowed */
    for (int r = 0; r < 256; ++r) cur[r] = 1;

    /* Perform n transitions (producing n+1 rows) */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) next[i] = 0;

        for (int r = 0; r < 256; ++r) {
            seq_t val = cur[r];
            if (!val) continue;
            for (int s = 0; s < 256; ++s) {
                if (allowed[r][s]) next[s] += val;
            }
        }
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all ending states */
    seq_t total = 0;
    for (int r = 0; r < 256; ++r) total += cur[r];

    /* free */
#ifdef __cplusplus
    delete[] cur;
    delete[] next;
#else
    free(cur);
    free(next);
#endif

    return total;
}
