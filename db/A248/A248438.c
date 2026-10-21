/*
 * A248438  Number of length 5+2 0..n arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 7‑element arrays over the alphabet {0,1,…,n}
 *   such that for every three consecutive elements (x,y,z) there exists
 *   an element among them equal to the average of the other two.
 *
 * This is implemented by dynamic programming over the last two elements of
 * the array.  The DP table is built iteratively from length 3 to length 7.
 * Each table entry can be at most 243, so we store it in an 8‑bit array.
 * The final answer is the sum of all table entries for length 7.
 *
 * The implementation uses two arrays of size (n+1)² to hold the DP values
 * for the current and the previous length.  Memory consumption is
 * roughly 2·(n+1)² bytes (≈200 MiB for n = 10 000).
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    const size_t len   = (size_t)(n + 1);      /* number of possible values */
    const size_t M     = len * len;            /* number of pair states */

    /* allocate two DP arrays of unsigned 8‑bit integers */
    uint8_t *prev = (uint8_t*)malloc(M);
    uint8_t *cur  = (uint8_t*)malloc(M);
    if (!prev || !cur) {
        free(prev); free(cur);
        return 0;
    }

    /* ---------- length 3 DP (base case) --------------- */
    for (idx_t y = 0; y <= n; ++y) {
        size_t base_y = (size_t)y * len;
        for (idx_t z = 0; z <= n; ++z) {
            int xs[3], cnt = 0;

            /* candidate first elements for the triple (x,y,z) */
            int v = 2 * y - z;
            if (v >= 0 && (size_t)v <= n) xs[cnt++] = v;

            v = 2 * z - y;
            if (v >= 0 && (size_t)v <= n) {
                int dup = 0;
                for (int k = 0; k < cnt; ++k) if (xs[k] == v) { dup = 1; break; }
                if (!dup) xs[cnt++] = v;
            }

            if (((y + z) & 1) == 0) {
                v = (y + z) >> 1;
                if (v >= 0 && (size_t)v <= n) {
                    int dup = 0;
                    for (int k = 0; k < cnt; ++k) if (xs[k] == v) { dup = 1; break; }
                    if (!dup) xs[cnt++] = v;
                }
            }

            prev[base_y + z] = (uint8_t)cnt;   /* dp[3][y][z] */
        }
    }

    /* ---------- lengths 4 through 7 ------------------- */
    for (int step = 4; step <= 7; ++step) {
        for (idx_t y = 0; y <= n; ++y) {
            size_t base_y = (size_t)y * len;
            for (idx_t z = 0; z <= n; ++z) {
                int xs[3], cnt = 0;

                /* possible preceding elements for the triple (x,y,z) */
                int v = 2 * y - z;
                if (v >= 0 && (size_t)v <= n) xs[cnt++] = v;

                v = 2 * z - y;
                if (v >= 0 && (size_t)v <= n) {
                    int dup = 0;
                    for (int k = 0; k < cnt; ++k) if (xs[k] == v) { dup = 1; break; }
                    if (!dup) xs[cnt++] = v;
                }

                if (((y + z) & 1) == 0) {
                    v = (y + z) >> 1;
                    if (v >= 0 && (size_t)v <= n) {
                        int dup = 0;
                        for (int k = 0; k < cnt; ++k) if (xs[k] == v) { dup = 1; break; }
                        if (!dup) xs[cnt++] = v;
                    }
                }

                uint8_t sum = 0;
                for (int k = 0; k < cnt; ++k) {
                    size_t idx = ((size_t)xs[k]) * len + y;
                    sum += prev[idx];
                }
                cur[base_y + z] = sum;    /* dp[step][y][z] */
            }
        }
        /* swap tables for next iteration */
        uint8_t *tmp = prev; prev = cur; cur = tmp;
    }

    /* ---------- compute final answer ------------------ */
    seq_t total = 0;
    for (size_t i = 0; i < M; ++i) {
        total += (seq_t)prev[i];
    }

    free(prev);
    free(cur);
    return total;
}
