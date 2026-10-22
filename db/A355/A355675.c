/*
 * A355675  a(0) = 0, and for any n > 0 and d = 1..9, a(10*n) = 10*a(n),
 *          a(10*n + d) = d - 10*a(n).
 * https://oeis.org/A355675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   not stated
 * Range:     i8 n <= 199, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0,
   a(10*n)     = 10*a(n),
   a(10*n+d)   = d - 10*a(n)   (d=1..9) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t cur = 0;
    idx_t m = n;
    idx_t power = 1;

    /* find highest power of 10 ≤ n */
    while (m >= 10) {
        m /= 10;
        power *= 10;
    }

    /* process digits from most significant to least */
    while (power > 0) {
        seq_t d = n / power;   /* current digit (0..9) */
        if (d == 0)
            cur = 10 * cur;
        else
            cur = d - 10 * cur;
        n %= power;
        power /= 10;
    }

    return cur;
}
