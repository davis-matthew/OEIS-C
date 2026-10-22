/*
 * A398514  Indices of record high-points in A398512.
 * https://oeis.org/A398514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) ≈ index of record high-points in A398512. The values are
 *            formed by concatenating decimal digits: - a(1) = 0 - a(2) = 11 -
 *            a(4) = 110 For odd n ≥ 3: a(n) = [9,8,…,L] followed by L, where
 *            L = 9 - (n-3)/2. For even n ≥ 6: a(n) = [1,0,9,8,…,L] followed
 *            by L, where L = 9 - (n-6)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle special small indices explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)11;
    if (n == 4) return (seq_t)110;

    /* For larger n, construct the number digit by digit. */
    seq_t val = (seq_t)0;

    if (n & 1) {  /* odd n ≥ 3 */
        idx_t k = (n - 3) >> 1;                 /* k = (n-3)/2 */
        int min_digit = 9 - (int)k;              /* L value */
        /* Append digits 9 down to L. */
        for (int d = 9; d >= min_digit; --d)
            val = val * (seq_t)10 + (seq_t)d;
        /* Repeat the last digit. */
        val = val * (seq_t)10 + (seq_t)min_digit;
    } else {     /* even n ≥ 6 */
        idx_t k = (n - 6) >> 1;                 /* k = (n-6)/2 */
        int min_digit = 9 - (int)k;              /* L value */
        /* Leading digits 1 and 0. */
        val = (seq_t)1;
        val = val * (seq_t)10 + (seq_t)0;
        /* Append digits 9 down to L. */
        for (int d = 9; d >= min_digit; --d)
            val = val * (seq_t)10 + (seq_t)d;
        /* Repeat the last digit. */
        val = val * (seq_t)10 + (seq_t)min_digit;
    }

    return val;
}
