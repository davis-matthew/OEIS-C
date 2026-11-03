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
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A374685: Numbers k such that the leaders of strictly increasing runs
   in the k‑th composition in standard order are identical. */
#include <stdbool.h>
#include <stdint.h>

static inline bool _ok_for_k(idx_t k) {
    seq_t parts[64];
    int len = 0;
    idx_t pos = 1;
    idx_t last = 0;
    idx_t temp = k;
    while (temp) {
        if (temp & 1ULL) {
            parts[len++] = (seq_t)(pos - last);
            last = pos;
        }
        temp >>= 1;
        pos++;
    }
    if (len == 0) return true;            /* empty composition */
    seq_t firstLeader = parts[len - 1];
    seq_t prev = firstLeader;
    for (int i = len - 2; i >= 0; --i) {
        seq_t part = parts[i];
        if (part <= prev) {
            if (part != firstLeader) return false;
        }
        prev = part;
    }
    return true;
}

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
