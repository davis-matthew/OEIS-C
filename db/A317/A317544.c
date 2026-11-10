/*
 * A317544  Positions of 0's in A317542, the formal inverse of the
 *          period-doubling sequence A096268.
 * https://oeis.org/A317544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (0-based) of the n-th 0 (n >= 0) in A317542,
 *            where A317542(0)=0, A317542(1)=1, A317542(2n)=0,
 *            A317542(4n+1)=A317542(2n-1) (n>=1), A317542(4n+3)=A317542(n);
 *            found by a linear scan counting zeros
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A317542_is_zero(idx_t m)
{
    for (;;) {
        if (m == 0) return 1;
        if (m == 1) return 0;
        if ((m & 1) == 0) return 1;
        if ((m & 3) == 1)
            m = (m - 3) / 2;      /* 2k-1 with k=(m-1)/4 */
        else
            m = (m - 3) / 4;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 0;; m++) {
        if (A317542_is_zero(m)) {
            if (count == n)
                return (seq_t)m;
            count++;
        }
    }
}
