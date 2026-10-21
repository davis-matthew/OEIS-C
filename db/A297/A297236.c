/*
 * A297236  Total variation of base-12 digits of n; see Comments.
 * https://oeis.org/A297236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of absolute differences between consecutive base‑12
 *            digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    unsigned char digits[65];
    int len = 0;

    if (t == 0) {                     /* a(0) is unused, but safe */
        return 0;
    }
    while (t != 0) {                   /* extract base‑12 digits (least significant first) */
        digits[len++] = (unsigned char)(t % 12);
        t /= 12;
    }

    seq_t var = 0;
    for (int i = 1; i < len; ++i) {    /* sum absolute differences between adjacent digits */
        idx_t diff = digits[i] > digits[i-1] ? digits[i] - digits[i-1]
                                            : digits[i-1] - digits[i];
        var += (seq_t)diff;
    }

    return var;
}
