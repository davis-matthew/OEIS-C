/*
 * A346688  Replace 4^k with (-1)^k in base-4 expansion of n.
 * https://oeis.org/A346688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum_{i=0}^{∞} (-1)^i * d_i, where d_i are the base‑4
 *            digits of n (least significant first)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;     /* accumulate the alternating digit sum */
    int   sign = 1;    /* +1 for the least significant digit, then alternate */
    idx_t m = n;       /* work on a copy of n */

    while (m != 0) {
        int digit = (int)(m & 3ULL); /* m % 4 (fast) */
        sum += (seq_t)sign * (seq_t)digit;
        sign = -sign;          /* alternate the sign */
        m >>= 2;               /* m /= 4 */
    }
    return sum;   /* a(0) = 0 handled automatically */
}
