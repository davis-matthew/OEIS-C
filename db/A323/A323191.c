/*
 * A323191  Expansion of (1 - x^5) / ((1 + x) * (1 + x^4)) in powers of x.
 * https://oeis.org/A323191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n + (n%4==0? (-1)^(n/4) : 0) for n>0; a(0)=1
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323191. */
seq_t a(idx_t n) {
    if (n == 0)
        return 1;
    seq_t res;
    if ((n & 1) == 0)
        res = 1;
    else
        res = -1;
    if ((n % 4) == 0) {
        if (((n / 4) & 1) == 0)
            res += 1;
        else
            res -= 1;
    }
    return res;
}
