/*
 * A297128  Numbers having an up-first zigzag pattern in base 4; see Comments.
 * https://oeis.org/A297128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having an up‑first zigzag pattern in base 4
 * (OEIS A297128).
 * No closed‑form formula exists; the function searches the positive
 * integers in increasing order and returns the n‑th qualifying number.
 */

#include <stdbool.h>
#include <stddef.h>

static bool up_first_zigzag(seq_t v)
{
    /* Extract base‑4 digits, least significant first.   */
    /* The magnitude of v may use up to 128 bits, so a 65‑byte
     * buffer is more than enough for the digit list.        */
    uint8_t digits[65];
    size_t len = 0;
    seq_t t = v;

    while (t != 0) {
        digits[len++] = (uint8_t)(t % 4); /* d = v mod 4 */
        t /= 4;                          /* v = v / 4   */
    }

    /* A single digit cannot form a zigzag.  */
    if (len < 2)
        return false;

    /* Scan the comparisons from most significant to least. */
    bool first = true;      /* first comparison not yet processed  */
    bool dir_up = true;     /* expected direction (true = up)      */

    for (size_t idx = len - 1; idx > 0; --idx) {
        uint8_t high = digits[idx];     /* more significant digit    */
        uint8_t low  = digits[idx - 1]; /* less significant digit     */

        if (high == low)                /* no equal adjacent digits    */
            return false;

        bool up = (high < low);         /* true if the comparison is up */

        if (first) {
            if (!up)                    /* must start with an up step  */
                return false;
            first = false;
            dir_up = true;
            continue;
        }

        /* When the direction changes, flip the expectation. */
        if (up != dir_up)
            dir_up = up;
    }

    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;  /* how many qualifying numbers have been seen */
    seq_t val = 1;    /* candidate value, start at 1 */

    while (1) {
        if (up_first_zigzag(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;  /* move to the next positive integer */
    }
}
