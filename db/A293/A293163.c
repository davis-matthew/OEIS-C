/*
 * A293163  a(n) = A010060(3n+1).
 * https://oeis.org/A293163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010060(3n+1) (A010060 is the Thue‑Morse sequence)
 * Range:     u8 n <= 84, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 3 + 1;
    unsigned int parity = 0;
    while (m)
    {
        parity ^= (unsigned int)(m & 1);
        m >>= 1;
    }
    return (seq_t)parity;
}
