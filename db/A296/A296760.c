/*
 * A296760  Numbers whose base-16 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9405 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose base‑16 digits have more rises than falls
   by brute‑force search. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return true if the number 'x' has more rises than falls in its
   hexadecimal representation. */
static bool has_more_rises(seq_t x)
{
    uint8_t buf[256];
    size_t len = 0;

    /* Extract the digits least‑significant first. */
    do {
        buf[len++] = (uint8_t)(x % (seq_t)16);
        x /= (seq_t)16;
    } while (x != (seq_t)0);

    if (len <= 1) {
        return false;                /* no rises or falls possible */
    }

    int rises = 0;
    int falls = 0;

    /* Compare digits from most‑significant to least‑significant.
       buf[len-1] is the most significant digit. */
    for (size_t i = len - 1; i > 0; --i) {
        uint8_t high = buf[i];
        uint8_t low  = buf[i - 1];
        if (high < low) {
            ++rises;
        } else if (high > low) {
            ++falls;
        }
    }
    return rises > falls;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)1;
    idx_t count = 0;

    for (;; ++x) {
        if (has_more_rises(x)) {
            ++count;
            if (count == n) {
                return x;
            }
        }
    }
}
