/*
 * A252488  Binary sequence starting with 1 and with run lengths given by the
 *          ruler sequence A001511.
 * https://oeis.org/A252488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = run-length binary sequence with run lengths from the *
 *            ruler sequence A001511 (starting with 1's, then alternating 0's)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   ruler
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ruler.h"

seq_t a(idx_t n) {
    /* n ≥ 1 */
    idx_t remaining = n;
    idx_t run = 1;
    uint8_t parity = 1;     /* 1 for run of 1's, 0 for run of 0's */
    while (1) {
        idx_t len = ruler(run);
        if (len >= remaining) {
            return parity ? (seq_t)1 : (seq_t)0;
        }
        remaining -= len;
        parity ^= 1;
        run++;
    }
}
