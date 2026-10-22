/*
 * A396812  Upper (1,1/2) midsequence of (floor[n/2]^2) and (ceiling[n/2]^2);
 *          see Comments.
 * https://oeis.org/A396812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( floor(n/2)^2 + floor((n+1)/2)^2 / 2 )
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* floor(n/2) */
    seq_t half = (seq_t)(n >> 1);
    seq_t s = half * half;                /* s = floor(n/2)^2 */

    /* floor((n+1)/2) */
    seq_t half_plus = (seq_t)(((n + 1) >> 1));
    seq_t t = half_plus * half_plus;      /* t = floor((n+1)/2)^2 */

    /* ceil(t/2) = (t + 1)/2 for integer t */
    return s + (t + 1) / 2;
}
