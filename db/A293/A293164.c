/*
 * A293164  a(n) = A010060(3n+2).
 * https://oeis.org/A293164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010060(3*n+2); A010060(m) is the Thue‑Morse sequence,
 *            i.e. 0 if the binary popcount of m is even, 1 if it is odd
 * Range:     u8 n <= 84, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 3 + 2;   /* 3*n+2  (must be done in seq_t) */
    unsigned int parity = 0;      /* parity of 1‑bit count */
    while (m) {
        parity ^= (unsigned int)(m & 1);  /* toggle when the LSB is 1 */
        m >>= 1;                          /* shift right */
    }
    return (seq_t)parity;          /* 0 or 1 fits into seq_t */
}
