/*
 * A296159  Sum of the smaller parts in the partitions of n into two distinct
 *          parts with the larger part odd.
 * https://oeis.org/A296159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} i * ((n-i) mod 2)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n ≥ 1.  We use the closed form:
       - if n is even: sum of odd i up to ⌊(n-1)/2⌋,
         which equals ceil(⌊(n-1)/2⌋/2)².
       - if n is odd : sum of even i up to ⌊(n-1)/2⌋,
         which equals floor(⌊(n-1)/2⌋/2) * (floor(⌊(n-1)/2⌋/2)+1).   */
    if (n < 1) return (seq_t)0;          /* safety for unexpected values */

    seq_t nseq = (seq_t)n;
    idx_t m_idx = (n - 1) / 2;           /* floor((n-1)/2) as idx_t (fits easily) */
    seq_t m = (seq_t)m_idx;

    if ((nseq & 1) == 0) {               /* n even: sum odd i */
        seq_t j = (m + 1) / 2;           /* ceil(m/2) */
        return j * j;
    } else {                             /* n odd: sum even i */
        seq_t t = m / 2;                 /* floor(m/2) */
        return t * (t + 1);
    }
}
