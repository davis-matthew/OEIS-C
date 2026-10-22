/*
 * A307710  a(n) is the determinant of the Vandermonde matrix of the digits of
 *          n in factorial base.
 * https://oeis.org/A307710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = ∏_{0<=i<j<L} (digit_j - digit_i) where digit_0 ...
 *            digit_{L-1} are the factorial‑base digits of n written from most
 *            significant to least (e.g. 22 = 3·3!+2·2!+0·1! → digits 3,2,0)
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 (empty product). */
    if (n == 0) return 1;

    /* Compute factorial‑base digits in least‑significant order
       using the standard incrementing modulus algorithm. */
    idx_t tmp = n;
    int digits_rev[20];          /* enough for 64‑bit n (max 18 digits). */
    int len = 0;
    for (int i = 1; tmp > 0; ++i) {
        digits_rev[len++] = (int)(tmp % (i + 1));
        tmp /= (i + 1);
    }

    /* Compute the Vandermonde determinant.
       Digits are used in order from most significant to least:
       digit_i = digits_rev[len-1-i]. */
    seq_t res = 1;
    int msb = len - 1;
    for (int i = 0; i < len; ++i) {
        seq_t d_i = (seq_t)digits_rev[msb - i];
        for (int j = i + 1; j < len; ++j) {
            seq_t d_j = (seq_t)digits_rev[msb - j];
            res *= (d_j - d_i);
        }
    }
    return res;
}
