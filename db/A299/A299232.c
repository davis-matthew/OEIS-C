/*
 * A299232  Ranks of {2,3}-power towers that start with 3; see Comments.
 * https://oeis.org/A299232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n for all n ≠ 2,3,5; a(2)=5, a(3)=7, a(5)=11
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 2: return (seq_t)5;
        case 3: return (seq_t)7;
        case 5: return (seq_t)11;
        default: return ((seq_t)n) * (seq_t)2;
    }
}
