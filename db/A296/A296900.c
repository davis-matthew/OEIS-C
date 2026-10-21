/*
 * A296900  Numbers whose base-16 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th integer whose base‑16 digits have an equal
   number of pits and peaks: #(pits) = #(peaks).
   The function enumerates integers, counts pits/peaks of each,
   and returns the n‑th one. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* number of qualifying values found so far */
    idx_t count = 0;
    /* current integer being tested */
    seq_t val = 1;

    /* buffer for base‑16 digits (little‑endian) */
    uint8_t digits[256];

    while (count < n) {
        /* decode val into digits[] */
        idx_t len = 0;
        seq_t tmp = val;
        if (tmp == 0) {
            digits[len++] = 0;
        } else {
            while (tmp != 0) {
                digits[len++] = (uint8_t)(tmp & (seq_t)0xF);
                tmp >>= 4;
            }
        }

        /* count pits and peaks */
        idx_t pits = 0, peaks = 0;
        if (len >= 3) {
            for (idx_t i = 1; i + 1 < len; ++i) {
                uint8_t left  = digits[i - 1];
                uint8_t cur   = digits[i];
                uint8_t right = digits[i + 1];
                if (left > cur && cur < right) {
                    ++pits;
                } else if (left < cur && cur > right) {
                    ++peaks;
                }
            }
        }

        /* if the condition holds, count it */
        if (pits == peaks) {
            ++count;
            if (count == n) {
                return val;
            }
        }

        ++val;
    }

    /* unreachable, but keep compiler quiet */
    return (seq_t)0;
}
