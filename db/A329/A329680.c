/*
 * A329680  Number of excursions of length n with Motzkin-steps consisting
 *          only of consecutive steps UH, HD and DU.
 * https://oeis.org/A329680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 1 for n==0 or n==1 or (n%3==0); otherwise a(n)=0
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n == 0 || n == 1 || (n % 3) == 0) ? (seq_t)1 : (seq_t)0;
}
