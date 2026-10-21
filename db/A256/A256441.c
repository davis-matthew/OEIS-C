/*
 * A256441  Binary representation of base-(i-1) expansion of -n: replace i-1
 *          with 2 in base-(i-1) expansion of -n.
 * https://oeis.org/A256441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 204, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A256441:
 *   a(n) = binary value obtained by writing -n in base -4, transforming
 *   each base-4 digit d (0..3) to the 4‑bit pattern 0000,0001,1100,1101,
 *   and then interpreting the concatenated bits as an unsigned binary number.
 *   (This is equivalent to the definition “replace i‑1 with 2 in the
 *   expansion of –n”.) */
#include <stddef.h>

seq_t a(idx_t n) {
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    /* Mapping from base‑4 digits to 4‑bit blocks */
    const seq_t map[4] = {0, 1, 12, 13};

    /* Temporary storage for the digits of -n in base -4 */
    uint8_t digits[128];   /* 128 digits is more than enough for all n up to 2^127 */
    size_t len = 0;

    /* Use signed 128‑bit integer to avoid overflow when negating large n */
    __int128 x = - (__int128) n;

    /* Convert -n to base -4, least‑significant digit first */
    while (x != 0) {
        __int128 r = x % (-4);
        if (r < 0)
            r += 4;                /* make residue 0..3 */
        digits[len++] = (uint8_t) r;
        x = (x - r) / (-4);
    }

    /* Build the final binary number from most‑significant digit to least. */
    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        res = (res << 4) | map[digits[i - 1]];
    }
    return res;
}
