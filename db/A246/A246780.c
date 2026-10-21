/*
 * A246780  Strictly increasing terms of the sequence A246778: a(1)=
 *          A246778(1) and for n>0 a(n+1) is next term greater than a(n) after
 *          that a(n) appears in A246778 for the first time.
 * https://oeis.org/A246780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 5 for all n > 15 (conjectured); a(1) = 2, a(2) = 3,
 *            a(3) = 4, a(4) = 6, a(5) = 8, a(6) = 9, a(7) = 11, a(8) = 12,
 *            a(9) = 13, a(10) = 14, a(11) = 15, a(12) = 16, a(13) = 17, a(14)
 *            = 18, a(15) = 19
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; n=0 is not used, but return 0 safely */
        return (seq_t)0;

    /* first 15 values are fixed by definition */
    if (n <= 15) {
        static const seq_t values[16] = {
            [0] = 0,          /* unused (offset starts at 1) */
            [1] = 2,
            [2] = 3,
            [3] = 4,
            [4] = 6,
            [5] = 8,
            [6] = 9,
            [7] = 11,
            [8] = 12,
            [9] = 13,
            [10] = 14,
            [11] = 15,
            [12] = 16,
            [13] = 17,
            [14] = 18,
            [15] = 19
        };
        return values[n];
    }

    /* for n > 15 the conjectured formula holds */
    return ((seq_t)n) + (seq_t)5;
}
