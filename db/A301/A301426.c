/*
 * A301426  Number of steps required in the worst case for three knights to
 *          find the princess in a castle with n rooms arranged in a line
 *          (Castle and princess puzzle).
 * https://oeis.org/A301426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=a(2)=1; for n≥3, a(n) = (n % 5 == 3) ? ((n-3)/5*2 + 1) :
 *            ((n-4)/5*2 + 2)
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2)
        return (seq_t)1;
    if (n % 5 == 3)
        return ((seq_t)(n - 3) / 5) * 2 + 1;
    return ((seq_t)(n - 4) / 5) * 2 + 2;
}
