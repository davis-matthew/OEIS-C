/*
 * A251890  Number of (n+2) X (4+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not 2 3 6 or 7 and every diagonal and antidiagonal
 *          sum 2 3 6 or 7.
 * https://oeis.org/A251890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) = a(n-1) + 4*a(n-3) - 4*a(n-4) for n>9, with
 *            initial values a(1)=1280, a(2)=530, a(3)=778, a(4)=1316,
 *            a(5)=1958, a(6)=2660, a(7)=4490, a(8)=7250, a(9)=10634
 * Range:     u8 none, u16 n <= 12, u32 n <= 36, u64 n <= 84, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_size = 0;

static const seq_t init_vals[] = {
    0,          /* placeholder for index 0 */
    (seq_t)1280,
    (seq_t)530,
    (seq_t)778,
    (seq_t)1316,
    (seq_t)1958,
    (seq_t)2660,
    (seq_t)4490,
    (seq_t)7250,
    (seq_t)10634
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined offset, return 0 */

    if (n >= memo_size) {
        idx_t new_size = n + 1;
        seq_t *new_memo = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_memo) {
            /* Allocation failed; caller should handle this if needed. */
            return (seq_t)0;
        }
        memo = new_memo;
        for (idx_t i = memo_size; i < new_size; ++i) {
            if (i <= 9) {
                memo[i] = init_vals[i];
            } else {
                memo[i] = memo[i-1] + 4 * memo[i-3] - 4 * memo[i-4];
            }
        }
        memo_size = new_size;
    }
    return memo[n];
}
