/*
 * A242602  Integers repeated thrice in a canonical order.
 * https://oeis.org/A242602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor((n+3)/6) * (-1)^floor((n+3)/3)
 * Range:     i8 n <= 764, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor((n+3)/6).  Use modular arithmetic to avoid
       overflowing the addition n+3. */
    idx_t q = n / 6;
    idx_t rem6 = n % 6;
    if (rem6 + 3 >= 6)   /* the +3 may carry into the next multiple of 6 */
        q++;

    /* Compute the sign: (-1)^floor((n+3)/3).  Since floor((n+3)/3)
       = n/3 + 1 for all n >= 0, we can use the parity of (n/3 + 1). */
    idx_t t = n / 3 + 1;     /* floor((n+3)/3) */
    seq_t sign = (t & 1) ? -1 : 1;   /* if t is odd -> -1, else +1 */

    return (seq_t)q * sign;
}
