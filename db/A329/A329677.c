/*
 * A329677  Number of excursions of length n with Motzkin-steps consisting
 *          only of consecutive steps UH, HD, and DH.
 * https://oeis.org/A329677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 1 if n==0||n==1||n==3||n==4, else 0
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n==0 || n==1 || n==3 || n==4) ? 1 : 0;
}
