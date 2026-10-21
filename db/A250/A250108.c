/*
 * A250108  n*(n-1)/2 mod 2 + n*(n-1)/2 - n*( (n-1) mod 2 ).
 * https://oeis.org/A250108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n-1)/2) mod 2 + n*(n-1)/2 - n*((n-1) mod 2)
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1, behavior below is irrelevant */
    seq_t T = ((seq_t)n) * ((seq_t)n - 1) / 2;  /* n*(n-1)/2 */
    seq_t r = ((seq_t)(n - 1)) & 1;              /* (n-1) mod 2 */
    return (T & 1) + T - ((seq_t)n) * r;        /* (T mod 2) + T - n*r */
}
