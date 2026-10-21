/*
 * A297265  Numbers whose base-8 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 8198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th positive integer whose base‑8 down‑variation
   equals its up‑variation.  The brute‑force method is adequate for
   the required range (≤ 10000). */
#include <stdbool.h>

static inline bool equal_variation(seq_t value) {
    seq_t dv = 0, uv = 0;
    int prev = -1;
    seq_t v = value;                     /* copy, will be consumed */
    while (v > 0) {
        int digit = (int)(v % 8);        /* single octal digit */
        if (prev == -1) {
            prev = digit;
        } else {
            if (digit > prev) {
                dv += digit - prev;
            } else if (digit < prev) {
                uv += prev - digit;
            }
            prev = digit;
        }
        v /= 8;
    }
    return dv == uv;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 0;
    while (count < n) {
        candidate += 1;                  /* test next integer */
        if (equal_variation(candidate)) {
            ++count;
        }
    }
    return candidate;
}
