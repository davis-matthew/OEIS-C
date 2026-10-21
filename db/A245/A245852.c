/*
 * A245852  Powers of 8 without the digit '0' in their decimal expansion.
 * https://oeis.org/A245852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   noZero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate powers of 8 and count those whose decimal expansion
 * contains no digit '0'; return the n‑th such power. */
#include <stdbool.h>
#include "../utils/noZero.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t val   = 1;  /* 8^0 */
    while (1) {
        if (noZero(val)) {
            ++count;
            if (count == n)
                return val;
        }
        val = val * 8;
    }
}
