/*
 * A241953  Number of possible representations of n as a sum of distinct
 *          positive integers from the Fibonacci-type sequences
 *          2,1,3,4,7,11,... and 0,3,3,6,9,15,... (A000032 and A022086).
 * https://oeis.org/A241953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as a sum of * distinct
 *            positive integers chosen from the union of the two *
 *            Fibonacci‑type sequences * A000032: 2,1,3,4,7,11,18,29,... *
 *            A022086: 0,3,3,6,9,15,24,39,... * (the zero term of A022086 is
 *            ignored because all summands must be * positive). The compu...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Count the number of ways to write n as a sum of distinct integers
 * taken from the described set.  Results are computed on the fly;
 * no memoisation is used because the procedure is already O(n·k)
 * with k≈O(log n) parts. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* the definition is only for n≥1 */
        return 0;

    /* ----- Mark all admissible parts not exceeding n ----- */
    bool *present = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!present)               /* very unlikely for the given limits */
        return 0;

    /* Sequence A000032: 2,1,3,4,7,11,...  */
    idx_t a1 = 2, a2 = 1;
    if (a1 <= n) present[(size_t)a1] = true;
    if (a2 <= n) present[(size_t)a2] = true;
    while (1) {
        idx_t next = a1 + a2;          /* next term, using 64‑bit arithmetic
                                         * to avoid overflow in seq_t */
        if (next > n) break;
        present[(size_t)next] = true;
        a1 = a2;
        a2 = next;
    }

    /* Sequence A022086: 0,3,3,6,9,15,24,39,...  (zero omitted) */
    idx_t b1 = 0, b2 = 3;
    if (b2 <= n) present[(size_t)b2] = true;
    while (1) {
        idx_t next = b1 + b2;
        if (next > n) break;
        if (next > 0) present[(size_t)next] = true;   /* skip the zero entry */
        b1 = b2;
        b2 = next;
    }

    /* ----- Collect the distinct parts into an array ----- */
    size_t capacity = 64;
    seq_t *parts = (seq_t *)malloc(capacity * sizeof(seq_t));
    size_t parts_cnt = 0;
    for (idx_t val = 1; val <= n; ++val) {
        if (present[(size_t)val]) {
            if (parts_cnt == capacity) {
                capacity <<= 1;
                parts = (seq_t *)realloc(parts, capacity * sizeof(seq_t));
            }
            parts[parts_cnt++] = (seq_t)val;
        }
    }
    free(present);

    /* ----- Subset‑sum DP: dp[x] = number of ways to make sum x ----- */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) {
        free(parts);
        return 0;
    }
    dp[0] = (seq_t)1;                    /* empty sum */

    for (size_t pi = 0; pi < parts_cnt; ++pi) {
        idx_t part = (idx_t)parts[pi];
        /* iterate downwards to keep each part distinct */
        for (idx_t i = n; i >= part; --i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    free(parts);
    return result;
}
