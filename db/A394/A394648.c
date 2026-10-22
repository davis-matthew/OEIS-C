/*
 * A394648  Rectangular array, read by descending antidiagonals: the offset
 *          sum array of the Thue-Morse sequence, A010060; see Comments.
 * https://oeis.org/A394648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = parity(n) + parity(d), where d =
 *            floor((sqrt(8*n+1)-1)/2), n' = n - d*(d+1)/2 (offset within
 *            diagonal), and parity(x) is the parity of the number of 1‑bits
 *            of x (the Thue–Morse value)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t d = 0;
    idx_t tri = 0;   /* tri = d*(d+1)/2, starting index of diagonal d */
    while (tri + d + 1 <= n) {
        tri += d + 1;
        d++;
    }
    idx_t k = n - tri;
    seq_t val =
        ((seq_t)((__builtin_popcountll((unsigned long long)d) & 1))) +
        ((seq_t)((__builtin_popcountll((unsigned long long)k) & 1)));
    return val;
}
