/*
 * A378299  Read the binary representation of n from the most to least
 *          significant bit then perform a cumulative XOR and store by reading
 *          from least to most significant bit.
 * https://oeis.org/A378299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A378299: 
 *   a(n) = gray-code formed by cumulatively XORing the binary digits
 *   of n (processed from least‑to‑most significant bit) and
 *   storing the resulting XOR values in reverse order.
 *
 * Algorithm:
 *   Let m = n, x = 0, out = 0.
 *   While m > 0:
 *     bit = m & 1;
 *     x ^= bit;            // cumulative XOR
 *     out = (out << 1) | x; // prepend new XOR value
 *     m >>= 1;
 *   Return out.
 */
seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t x = 0;
    seq_t out = 0;

    while (m > 0) {
        /* bit is either 0 or 1, cast to seq_t for XOR/shift */
        seq_t bit = (m & 1);
        x ^= bit;
        out = (out << 1) | x;
        m >>= 1;
    }

    return out;
}
