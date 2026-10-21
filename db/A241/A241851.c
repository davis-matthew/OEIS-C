/*
 * A241851  a(n) = n^2 + 21.
 * https://oeis.org/A241851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 21
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence type before any arithmetic
       to avoid overflow in the intermediate multiplication when
       seq_t has small width (e.g., uint8_t). */
    seq_t n_seq = (seq_t)n;
    return n_seq * n_seq + (seq_t)21;
}
