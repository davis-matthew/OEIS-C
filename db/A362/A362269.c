/*
 * A362269  a(1) = 1, then subtract, add, and multiply 2, 3, 4; 5, 6, 7; ...
 *          in that order.
 * https://oeis.org/A362269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) - n if n mod 3 = 2, a(n-1) +
 *            n if n mod 3 = 0, a(n-1) * n if n mod 3 = 1
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 27, i64 n <= 45, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset: a(1) = 1 */
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        switch (i % 3) {
            case 2: /* subtract */
                val -= (seq_t)i;
                break;
            case 0: /* add */
                val += (seq_t)i;
                break;
            default: /* multiply (i % 3 == 1) */
                val *= (seq_t)i;
                break;
        }
    }
    return val;
}
