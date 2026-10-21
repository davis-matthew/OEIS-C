/*
 * A257534  Numbers n such that the decimal expansions of both n and n^2 have
 *          4 as the digit with the smallest value and 7 as the digit with the
 *          largest value.
 * https://oeis.org/A257534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 744…74476, i.e. for n≥2 it is (n−1) copies of the string "744"
   followed by "76"; a(1)=74.  The digits 4,5,6,7 appear and the
   minimum digit is 4, the maximum digit is 7 for both n and n^2. */
seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence is defined for n≥1 */
        return 0;

    /* the first term is a special case */
    if (n == 1)
        return (seq_t)74;

    seq_t result = (seq_t)0;
    idx_t k = n - 1;                 /* number of repetitions of "744" */

    /* append "744" k times */
    for (idx_t i = 0; i < k; ++i) {
        result = result * (seq_t)10 + (seq_t)7;
        result = result * (seq_t)10 + (seq_t)4;
        result = result * (seq_t)10 + (seq_t)4;
    }

    /* finally append "76" */
    result = result * (seq_t)10 + (seq_t)7;
    result = result * (seq_t)10 + (seq_t)6;

    return result;
}
