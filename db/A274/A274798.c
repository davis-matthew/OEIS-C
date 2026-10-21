/*
 * A274798  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) (0,-1) or (-1,0) and new values introduced
 *          in order 0..2.
 * https://oeis.org/A274798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) + 2*a(n-3) with initial values
 *            a(1)=2, a(2)=8, a(3)=30
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    if (n == 1) return 2;
    if (n == 2) return 8;
    if (n == 3) return 30;

    seq_t p3 = 2;                  /* a(n-3) */
    seq_t p2 = 8;                  /* a(n-2) */
    seq_t p1 = 30;                 /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 2 * p1 + 6 * p2 + 2 * p3;  /* recurrence */
        p3 = p2;
        p2 = p1;
        p1 = next;
    }
    return p1;
}
