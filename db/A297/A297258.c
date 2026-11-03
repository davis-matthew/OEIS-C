/*
 * A297258  Numbers whose base-6 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8005 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: base‑6 numbers whose down‑variation exceeds up‑variation.
   a(n) is the nth positive integer satisfying DV(n,6) > UV(n,6).  */

/* Requires <stdbool.h>, <stdlib.h>, <stdint.h> */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: returns true iff x in base‑6 has greater down‑variation than up‑variation. */
static bool has_greater_down_variation(seq_t x)
{
    /* Numbers with a single base‑6 digit have DV = UV = 0. */
    if (x < (seq_t)6) return false;

    seq_t tmp = x;
    size_t cap = 8, len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap);

    /* Extract base‑6 digits (least significant first). */
    while (tmp > (seq_t)0) {
        if (len == cap) {
            cap *= 2;
            digits = (uint8_t *)realloc(digits, cap);
        }
        digits[len++] = (uint8_t)(tmp % (seq_t)6);
        tmp /= (seq_t)6;
    }

    if (len <= 1) {  /* Should not happen for x >= 6, but keep safe. */
        free(digits);
        return false;
    }

    seq_t dv = 0;
    seq_t uv = 0;
    for (size_t i = len - 1; i > 0; --i) {
        int diff = (int)digits[i - 1] - (int)digits[i]; /* next - current, most → least */
        if (diff > 0) uv += (seq_t)diff;          /* increase */
        else if (diff < 0) dv += (seq_t)(-diff);   /* decrease */
    }

    free(digits);
    return dv > uv;
}

/* Main function: returns the nth term of A297258. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;

    /* Search all positive integers until we find the nth qualifying one. */
    for (seq_t x = 1; /* no upper bound */; ++x) {
        if (has_greater_down_variation(x)) {
            ++count;
            if (count == n) return x;
        }
    }
}
