/*
 * A371459  For any positive integer with binary digits (b_1, ..., b_w) (where
 *          b_1 = 1), the binary digits of a(n), possibly with leading zeros,
 *          are (b_2, b_4, ..., b_{floor(w/2) * 2}); a(0) = 0.
 * https://oeis.org/A371459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = binary number formed by taking the * even-indexed bits of
 *            the binary representation of n * (counted from the
 *            most‑significant bit). Equivalent to * keeping bits b_2, b_4, …
 *            of (b_1, …, b_w) where b_1 = 1. * a(0) = 0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the sequence is defined to be 0. */
    if (n == 0u) {
        return (seq_t)0;
    }

    /* Compute the number of bits w of n.  This is ⌊log₂ n⌋ + 1. */
    idx_t w   = 0;
    idx_t tmp = n;
    while (tmp != 0u) {
        tmp >>= 1u;
        ++w;
    }

    /* Extract the even‑indexed bits (excluding the leading bit). */
    seq_t res = (seq_t)0;
    for (idx_t pos = 1; pos <= w; ++pos) {
        /* bit at position pos (1 = most‑significant). */
        idx_t bit = (n >> (w - pos)) & 1u;

        /* Skip the leading bit (pos == 1). */
        if (pos == 1) {
            continue;
        }

        /* Keep only even positions. */
        if ((pos & 1u) == 0u) {
            /* Append the bit to the result:
             * res = res * 2 + bit
             * This works for any seq_t, including arbitrary‑precision. */
            res = res * (seq_t)2 + (seq_t)bit;
        }
    }

    return res;
}
