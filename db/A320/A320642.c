/*
 * A320642  Number of 1's in the base-(-2) expansion of -n.
 * https://oeis.org/A320642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1's in the base (-2) expansion of -n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Work in a signed 128‑bit type that can hold numbers ≥ -(2⁶⁴−1) */
    signed __int128 m = -(signed __int128)n;  /* m = -n  (might be negative) */
    unsigned long long cnt = 0;               /* count of 1’s in the representation */

    while (m != 0) {
        if (m & 1)           /* remainder when divided by -2 is 1 */
            ++cnt;
        /* next value: m = -(m - r) / 2, where r = m & 1 */
        m = -(m - (m & 1)) / 2;
    }

    return (seq_t)cnt;
}
