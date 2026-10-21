/*
 * A271876  Numbers n such that 3^n is not of the form x^3 + y^3 + z^3 where
 *          x, y, z > 0.
 * https://oeis.org/A271876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth term of sequence A271876 (numbers n such that 3^n is
 *            not a sum of three positive cubes)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is known to be finite; the only terms are 0,2,3,5,8,11,14,17,20. */
seq_t a(idx_t n)
{
    static const seq_t seq_values[] = {0, 2, 3, 5, 8, 11, 14, 17, 20};
    if (n == 0) {
        return (seq_t)0;  /* undefined for index 0, return 0 */
    }
    if (n <= sizeof(seq_values)/sizeof(seq_values[0])) {
        return seq_values[n-1];
    }
    /* For n beyond the length of the finite sequence, return 0 (no further terms). */
    return (seq_t)0;
}
