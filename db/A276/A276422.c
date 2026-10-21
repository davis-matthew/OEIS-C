/*
 * A276422  Triangle read by rows: T(n,k) is the number of partitions of n for
 *          which the sum of its odd singletons is k (0<=k<=n). A singleton in
 *          a partition is a part that occurs exactly once.
 * https://oeis.org/A276422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(r,k) is the number of partitions of r
 *            * with the sum of its odd singletons equal to k, read in
 *            row-major order * of the triangle defined by the OEIS entry
 *            A276422
 * Range:     u8 n <= 252, u16 n <= 1484, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build the whole triangle up to a safe maximum row so that indices up
 * to about 10000 are covered.  Since the cumulative number of terms up
 * to row r is (r+1)(r+2)/2, rows up to 200 already provide more than
 * 20 000 terms and thus cover all required indices. */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define MAX_ROW 200   /* enough to cover indices up to >20000 */

static seq_t T[MAX_ROW + 1][MAX_ROW + 1] = {0};

/* Helper: integer square root for 64‑bit values.
 * Returns floor(sqrt(x)). */
static idx_t isqrt_idx(idx_t x) {
    if (x == 0 || x == 1) return x;
    double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Compute the table T if it has not been computed yet. */
static void ensure_triangle(void) {
    static bool initialized = false;
    if (initialized) return;

    /* DP arrays for incremental construction */
    static seq_t dp_prev[MAX_ROW + 1][MAX_ROW + 1];
    static seq_t dp_next[MAX_ROW + 1][MAX_ROW + 1];

    /* initialize dp_prev[0][0] = 1, rest zero */
    for (idx_t i = 0; i <= MAX_ROW; ++i)
        for (idx_t j = 0; j <= MAX_ROW; ++j)
            dp_prev[i][j] = 0;
    dp_prev[0][0] = 1;

    for (idx_t part = 1; part <= MAX_ROW; ++part) {
        /* zero dp_next */
        for (idx_t i = 0; i <= MAX_ROW; ++i)
            for (idx_t j = 0; j <= MAX_ROW; ++j)
                dp_next[i][j] = 0;

        for (idx_t n = 0; n <= MAX_ROW; ++n) {
            for (idx_t s = 0; s <= MAX_ROW; ++s) {
                seq_t val = dp_prev[n][s];
                if (val == 0) continue;
                idx_t maxm = (MAX_ROW - n) / part;
                for (idx_t m = 0; m <= maxm; ++m) {
                    idx_t new_n = n + m * part;
                    idx_t new_s = s + ((m == 1 && (part & 1)) ? part : 0);
                    if (new_s > MAX_ROW) continue;
                    dp_next[new_n][new_s] += val;
                }
            }
        }
        /* swap dp_next into dp_prev for next iteration */
        for (idx_t i = 0; i <= MAX_ROW; ++i)
            for (idx_t j = 0; j <= MAX_ROW; ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* copy the results into the global table T */
    for (idx_t r = 0; r <= MAX_ROW; ++r) {
        for (idx_t k = 0; k <= r; ++k)
            T[r][k] = dp_prev[r][k];
    }

    initialized = true;
}

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (isqrt_idx(8 * n + 1) - 1) / 2;
    idx_t first_in_row = r * (r + 1) / 2;
    idx_t k = n - first_in_row;
    if (k > r) return 0;   /* safety check */

    ensure_triangle();
    return T[r][k];
}
