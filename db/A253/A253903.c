/*
 * A253903  The characteristic function of square pyramidal numbers.
 * https://oeis.org/A253903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a square pyramidal number of the form
 *            m(m+1)(2m+1)/6 for some integer m≥0; otherwise a(n) = 0
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   square_pyramidal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/square_pyramidal.h"

/* Return the characteristic value at index n. */
seq_t a(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t val = square_pyramidal(mid);
        if (val == n) {
            return 1;
        } else if (val < n) {
            lo = mid + 1;
        } else {
            if (mid == 0)  /* prevents infinite loop when mid==0 */
                break;
            hi = mid - 1;
        }
    }
    return 0;
}
