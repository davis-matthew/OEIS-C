/*
 * A294924  Numbers n such that the whole sequence of the first n terms of
 *          A293699 is a palindrome.
 * https://oeis.org/A294924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=5, a(4)=7, a(n)=37*n-159 for all n≥5
 *            (numbers n such that the first n terms of A293699 form a
 *            palindrome)
 * Range:     u8 n <= 11, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four exceptional terms */
    if (n <= 4) {
        switch ((int)n) {
            case 1: return 1;
            case 2: return 3;
            case 3: return 5;
            case 4: return 7;
            default: return 0; /* unreachable */
        }
    }
    /* for n≥5 a(n)=37*n-159 */
    return (seq_t)37 * (seq_t)n - (seq_t)159;
}
