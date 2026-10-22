/*
 * A333124  a(n) is the number of square-subwords in the binary representation
 *          of n.
 * https://oeis.org/A333124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9933, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count square-subwords in the binary representation of n
 * (brute‑force enumeration of all even‑length substrings). */
seq_t a(idx_t n)
{
    /* n = 0 has binary “0”, which contains no non‑empty adjacent equal
       substrings, so the answer is 0. */
    if (n == 0)
        return 0;

    /* Determine the bit length of n (without leading zeros). */
    idx_t len = 0;
    idx_t t = n;
    while (t) {
        t >>= 1;
        ++len;
    }

    seq_t count = 0;

    /* Enumerate all even lengths l of a potential square‑subword. */
    for (idx_t l = 2; l <= len; l += 2) {
        idx_t k = l >> 1;                /* half‑length of the subword */
        if (k == 0)
            continue;                    /* should not happen for l>=2 */

        /* Mask to isolate k bits. */
        idx_t mask = (((idx_t)1 << k) - 1);

        /* Starting position s of the first half (0‑based from MSB). */
        idx_t max_start = len - l;
        for (idx_t s = 0; s <= max_start; ++s) {
            idx_t shift1 = len - (s + k); /* shift to align first half LSB */
            idx_t shift2 = len - (s + l); /* shift to align second half LSB */

            idx_t half1 = (n >> shift1) & mask;
            idx_t half2 = (n >> shift2) & mask;

            if (half1 == half2)
                ++count;
        }
    }

    return count;
}
