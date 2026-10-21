/*
 * A290825  Least base-3 digit of n.
 * https://oeis.org/A290825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least base-3 digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 by definition */
    seq_t min_digit = 2;           /* maximum possible digit in base 3 */
    idx_t x = n;
    while (x != 0) {
        seq_t d = (seq_t)(x % 3);  /* current base‑3 digit */
        if (d < min_digit) {
            min_digit = d;
            if (min_digit == 0)   /* can't get any smaller */
                break;
        }
        x /= 3;                   /* shift to next digit */
    }
    return min_digit;
}
