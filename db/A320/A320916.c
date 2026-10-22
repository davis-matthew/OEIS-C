/*
 * A320916  Consider A010060 as a 2-adic number ...100110010110, then a(n) is
 *          its approximation up to 2^n.
 * https://oeis.org/A320916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n-1} A010060(i)*2^i where A010060(i) =
 *            popcount(i) mod 2 (Thue–Morse sequence)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* Build the binary number bit by bit: for i = n-1 .. 0
       the bit t(i) becomes the new LSB after a left shift. */
    for (idx_t i = n; i > 0; ) {
        --i;
        /* popcount of i modulo 2: Thue–Morse bit */
        unsigned bit = __builtin_popcountll((unsigned long long)i) & 1U;
        res = (res << 1) | bit;
    }
    return res;
}
