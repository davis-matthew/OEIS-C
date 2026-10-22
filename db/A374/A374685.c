/*
 * A374685  Numbers k such that the leaders of strictly increasing runs in the
 *          k-th composition in standard order are identical.
 * https://oeis.org/A374685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _ok_for_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A374685: Numbers k such that the leaders of strictly increasing runs
   in the k‑th composition in standard order are identical. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_ok_for_k.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined, but harmless */
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        if (_ok_for_k(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
    return (seq_t)k;                       /* unreachable */
}
