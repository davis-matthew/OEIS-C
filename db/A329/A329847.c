/*
 * A329847  Beatty sequence for (3+sqrt(89))/8.
 * https://oeis.org/A329847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*( (3+sqrt(89))/8 ))
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root: returns ⌊√x⌋ for any seq_t. */
static inline seq_t _floor_sqrt(seq_t x) {
    seq_t lo = 0, hi = x;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset 1, but handle n==0 gracefully */

    seq_t nn   = (seq_t)n;
    seq_t prod = nn * nn;          /* n² */
    seq_t val  = prod * (seq_t)89; /* 89·n² */
    seq_t s    = _floor_sqrt(val); /* ⌊n·√89⌋   */
    seq_t ans  = ( (seq_t)3 * nn + s ) / (seq_t)8;
    return ans;
}
