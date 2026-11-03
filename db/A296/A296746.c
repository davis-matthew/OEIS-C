/*
 * A296746  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose base‑11 digit string has
 *            fewer rises than falls (i.e., rise < fall). The sequence is
 *            defined by direct examination of each integer, so this
 *            implementation simply enumerates successive integers until the
 *            n‑th qualifying number is found
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline int in_A296746(seq_t x)
{
    /* Return 1 if 'x' (in base 11) has fewer rises than falls,
       otherwise 0.  Single‑digit numbers are excluded because
       rises = falls = 0, so rises < falls does not hold. */

    if (x < (seq_t)11) return 0;  /* single‑digit case */

    /* Extract digits in least‑significant‑first order. */
    unsigned char lsd[64];      /* enough for up to ~40 digits */
    size_t len = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        lsd[len++] = (unsigned char)(t % (seq_t)11);
        t /= (seq_t)11;
    }

    if (len <= 1) return 0;      /* safety: should not happen */

    /* Count rises and falls while traversing from most‑significant
       to least‑significant digits.  We'll compare consecutive
       digits in the order they appear in the base‑11 string. */
    unsigned rising = 0, falling = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        unsigned d0 = lsd[len - 1 - i];         /* most to least */
        unsigned d1 = lsd[len - 2 - i];
        if (d0 < d1) rising++;
        else if (d1 < d0) falling++;
    }
    return (rising < falling) ? 1 : 0;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = (seq_t)1;
    while (1) {
        if (in_A296746(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
