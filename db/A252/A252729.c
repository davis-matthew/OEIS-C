/*
 * A252729  Start with 1, add 1, subtract 2, multiply by 3, add 4, subtract 5,
 *          multiply by 6, add 7, etc.
 * https://oeis.org/A252729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 27, i64 n <= 48, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1
 *   For k ≥ 1:
 *     let r = k mod 3
 *     if r == 1:  a(k+1) = a(k) + k
 *     if r == 2:  a(k+1) = a(k) – k
 *     if r == 0:  a(k+1) = a(k) * k
 */
seq_t a(idx_t n)
{
    /* Handle the offset; a(1) is the first term */
    if (n <= 1)
        return (seq_t)1;

    seq_t v = (seq_t)1;        /* a(1) */
    for (idx_t k = 1; k < n; ++k) {
        seq_t op = (seq_t)k;   /* the integer used in the operation */
        switch ((int)(k % 3)) {
            case 1:  v += op; break;  /* add k */
            case 2:  v -= op; break;  /* subtract k */
            case 0:  v *= op; break;  /* multiply by k */
        }
    }
    return v;
}
