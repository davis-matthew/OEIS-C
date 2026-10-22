/*
 * A356647  Concatenation of runs {y..x} for each x>=1, using each y from 1 to
 *          x before moving on to the next value for x.
 * https://oeis.org/A356647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000120(A087118(n + 1))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   T_cube, cum_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th element of the concatenated runs {y..x} for each x>=1, y=1..x. */

#include <stdint.h>
#include "../utils/T_cube.h"
#include "../utils/cum_run.h"

seq_t a(idx_t n)
{
    /* binary search for smallest x with T(x) >= n */
    idx_t low = 1, high = 1;
    while (T_cube(high) < n) {
        if (high > ((idx_t)-1) / 2) {
            high = (idx_t)-1;   /* avoid overflow */
            break;
        }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (T_cube(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t x = low;

    /* offset within block x (1‑based) */
    unsigned __int128 Tprev = T_cube(x - 1);
    idx_t offset = (idx_t)(n - Tprev);   /* offset ∈ [1, x*(x+1)/2] */
    idx_t r = offset - 1;                /* 0‑based index inside the block */

    /* find the run index y such that cum_run(y,x) > r */
    idx_t lo = 1, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cum_run(mid, x) > r)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t y = lo;

    unsigned __int128 cum_prev = cum_run(y - 1, x);
    idx_t p = r - (idx_t)cum_prev;       /* position inside the run (0‑based) */

    return (seq_t)(y + p);
}
