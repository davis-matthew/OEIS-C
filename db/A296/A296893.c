/*
 * A296893  Numbers whose base-13 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑13 digits d(i)
 *            satisfy #(pits) < #(peaks) (pit: d(i‑1) > d(i) < d(i+1); peak:
 *            d(i‑1) < d(i) > d(i+1))
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BASE 13
#define MAX_DIGITS 128   /* plenty for 128‑bit numbers */

static bool has_pits_lt_peaks(seq_t value)
{
    if (value == 0)            /* zero has no digits */
        return false;

    uint8_t digits[MAX_DIGITS];
    size_t size = 0;
    seq_t x = value;

    /* Extract base‑13 digits (least significant first) */
    while (x != 0 && size < MAX_DIGITS) {
        digits[size++] = (uint8_t)(x % (seq_t)BASE);
        x /= (seq_t)BASE;
    }
    if (x != 0)                 /* number too large for our buffer */
        return false;

    if (size < 3)                /* fewer than 3 digits → pits = peaks = 0 */
        return false;

    unsigned pits  = 0;
    unsigned peaks = 0;
    for (size_t i = 1; i + 1 < size; ++i) {
        uint8_t left  = digits[i - 1];
        uint8_t mid   = digits[i];
        uint8_t right = digits[i + 1];
        if (left > mid && mid < right)
            ++pits;
        else if (left < mid && mid > right)
            ++peaks;
    }
    return pits < peaks;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* defined only for n >= 1 */
        return 0;

    /* Compute the maximum representable value of seq_t */
    seq_t seq_t_max = (seq_t)(~(seq_t)0);

    idx_t found = 0;
    for (idx_t candidate = 1; candidate <= (idx_t)seq_t_max; ++candidate) {
        if (has_pits_lt_peaks((seq_t)candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
    }
    /* If we run out of representable values before reaching n, return 0 */
    return 0;
}
