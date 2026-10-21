/*
 * A284803  Write in base k, complement, reverse. Case k = 6.
 * https://oeis.org/A284803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse( complement(base6(n)) ) Complement means each
 *            base‑6 digit d is replaced by 5‑d. After reversing the digit
 *            order, interpret the resulting base‑6 number as a decimal
 *            integer
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case for n == 0: base‑6 representation is "0" → complement "5". */
    if (n == (idx_t)0)
        return (seq_t)5;

    seq_t result = (seq_t)0;
    while (n != (idx_t)0) {
        idx_t digit = n % (idx_t)6;   /* least‑significant base‑6 digit */
        n /= (idx_t)6;
        result = result * (seq_t)6 + (seq_t)(5 - digit);  /* reverse & complement */
    }
    return result;
}
