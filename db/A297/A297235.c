/*
 * A297235  Up-variation of the base-12 digits of n; see Comments.
 * https://oeis.org/A297235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297235: up-variation of the base-12 digits of n */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* for n=0 (outside the offset) return 0 – harmless */
    if (n == 0)
        return 0;

    uint8_t digs[20];      /* enough for 64‑bit numbers in base 12 */
    size_t len = 0;
    idx_t tmp = n;

    /* extract base‑12 digits, least significant first */
    while (tmp != 0) {
        digs[len++] = (uint8_t)(tmp % 12);
        tmp /= 12;
    }

    /* compute the up‑variation: sum (low - high) when high < low */
    seq_t sum = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        uint8_t low  = digs[i];
        uint8_t high = digs[i + 1];
        if (high < low)
            sum += (seq_t)(low - high);
    }

    return sum;
}
