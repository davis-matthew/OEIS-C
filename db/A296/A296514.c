/*
 * A296514  Positive numbers whose decimal expansion contains a 3-digit
 *          substring abc such that c > a > b.
 * https://oeis.org/A296514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer whose decimal representation contains a 3‑digit
   substring abc with c > a > b. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Check if x contains a 3‑digit substring abc with c > a > b. */
static bool has_property(seq_t x)
{
    seq_t t = x;
    uint8_t digits[64];
    size_t len = 0;

    /* Extract decimal digits (least significant first). */
    while (t > 0) {
        digits[len++] = (unsigned)(t % 10);
        t /= 10;
    }

    if (len < 3) return false;

    /* Scan all triples from most significant to least. */
    for (size_t i = 0; i + 3 <= len; ++i) {
        seq_t a = digits[len - 1 - i];
        seq_t b = digits[len - 2 - i];
        seq_t c = digits[len - 3 - i];
        if (c > a && a > b) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is a safe dummy. */

    seq_t val = 102;                /* first eligible number. */
    idx_t count = 0;

    while (1) {
        if (has_property(val)) {
            ++count;
            if (count == n) return val;
        }
        ++val;                      /* advance to next integer. */
    }
}
