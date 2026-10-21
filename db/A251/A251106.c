/*
 * A251106  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=37; a(n) = number of (n+1)×2 arrays with entries 0..2 *
 *            such that for every 2×2 subblock the maximum of its diagonal
 *            elements * does not exceed the absolute difference of its
 *            antidiagonal elements
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

static const uint8_t TRANS[9][9] = {
    /* row 0: (0,0) -> allowed to rows 0,3,4,6,7,8 */
    {1,0,0,1,1,0,1,1,1},
    /* row 1: (0,1) -> allowed to rows 0,1,3,6,7 */
    {1,1,0,1,0,0,1,1,0},
    /* row 2: (0,2) -> allowed to rows 0,1,2,3,4,6 */
    {1,1,1,1,1,0,1,0,0},
    /* row 3: (1,0) -> allowed to rows 3,4,6,7,8 */
    {0,0,0,1,1,0,1,1,1},
    /* row 4: (1,1) -> allowed to rows 0,1,6,7 */
    {1,1,0,0,0,0,1,1,0},
    /* row 5: (1,2) -> allowed to rows 0,1,2,3,4 */
    {1,1,1,1,1,0,0,0,0},
    /* row 6: (2,0) -> allowed to rows 6,7,8 */
    {0,0,0,0,0,0,1,1,1},
    /* row 7: (2,1) -> no outgoing transitions */
    {0,0,0,0,0,0,0,0,0},
    /* row 8: (2,2) -> allowed to rows 0,1,2 */
    {1,1,1,0,0,0,0,0,0}
};

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* sequence is defined for n >= 1 */

    seq_t cur[9], next[9];
    for (int i = 0; i < 9; ++i) cur[i] = (seq_t)1;  /* any first row is allowed */

    for (idx_t step = 0; step < n; ++step) {
        for (int j = 0; j < 9; ++j) next[j] = (seq_t)0;
        for (int i = 0; i < 9; ++i) {
            seq_t w = cur[i];
            if (!w) continue;
            for (int j = 0; j < 9; ++j) if (TRANS[i][j]) next[j] += w;
        }
        for (int i = 0; i < 9; ++i) cur[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 9; ++i) total += cur[i];
    return total;
}
