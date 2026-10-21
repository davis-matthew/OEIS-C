/*
 * A270968  Reduced 5x+1 function R applied to the odd integers: a(n) =
 *          R(2n-1), where R(k) = (5k+1)/2^r, with r as large as possible.
 * https://oeis.org/A270968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part of (5*n - 2)
 * Range:     u8 n <= 51, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = ((seq_t)5) * ((seq_t)n) - (seq_t)2;
    while ((val % (seq_t)2) == 0) {
        val = val / (seq_t)2;
    }
    return val;
}
