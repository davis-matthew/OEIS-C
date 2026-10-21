/*
 * A261064  a(n) = (3^n-1)*(n+1)/4.
 * https://oeis.org/A261064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n-1)*(n+1)/4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  compute 3^n  */
    seq_t pow3n = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow3n *= (seq_t)3;
    }

    seq_t diff = pow3n - (seq_t)1;          /*   3^n – 1   */

    if ((n & 1) == 0) {                     /* n even  */
        /*  (3^n-1) is divisible by 4  */
        seq_t part = diff / (seq_t)4;
        return part * ((seq_t)n + (seq_t)1);
    } else {                                 /* n odd   */
        /* divide both factors by 2  */
        seq_t part1 = diff / (seq_t)2;
        seq_t part2 = ((seq_t)n + (seq_t)1) / (seq_t)2;
        return part1 * part2;
    }
}
