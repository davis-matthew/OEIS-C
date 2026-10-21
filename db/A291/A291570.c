/*
 * A291570  The arithmetic function uhat(n,5,8).
 * https://oeis.org/A291570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Piecewise definition matching OEIS A291570 a(n) = -23 for n =
 *            1,2,3,4,7,8,9,11 a(n) = -25 for n = 5,10 a(n) = -24 for n = 6
 *            a(n) = -2 * n for n >= 12
 * Range:     i8 n <= 64, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first 11 indices with a table */
    if (n < 12) {
        switch ((uint64_t)n) {
            case 1: case 2: case 3: case 4:
            case 7: case 8: case 9: case 11:
                return (seq_t)(-23);
            case 5: case 10:
                return (seq_t)(-25);
            case 6:
                return (seq_t)(-24);
            default:
                return (seq_t)(-23); /* safety fallback */
        }
    }

    /* For n >= 12: a(n) = -2*n */
    unsigned long long tmp = 2ULL * (unsigned long long)n;  /* avoid overflow on idx_t */
    return -(seq_t)tmp;
}
