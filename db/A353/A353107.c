/*
 * A353107  Base-9 representation of A007908(n).
 * https://oeis.org/A353107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007095(A007908(n)) i.e., a(n) is the base‑9
 *            representation of the decimal concatenation 1‖2‖…‖n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the decimal concatenation 1‖2‖…‖n as a seq_t value m. */
    seq_t m      = (seq_t)0;
    seq_t pow10  = (seq_t)10;             /* 10^digits_of_i */
    idx_t threshold = (idx_t)10;          /* next power of 10 that increases digits */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        if (i == threshold)              /* the number of digits of i grows */
        {
            pow10 = pow10 * (seq_t)10;
            threshold = threshold * (idx_t)10;
        }

        /* Append i to m: m = m * 10^digits_of_i + i */
        m = m * pow10 + (seq_t)i;
    }

    /* Convert the decimal value m to base-9, storing the result as a decimal integer. */
    seq_t result = (seq_t)0;
    seq_t place  = (seq_t)1;              /* current decimal place value */

    while (m > (seq_t)0)
    {
        seq_t rem = m % (seq_t)9;        /* least significant base-9 digit (0–8) */
        result = result + rem * place;  /* append digit in decimal */
        place  = place * (seq_t)10;      /* shift to the next decimal place */
        m = m / (seq_t)9;                /* drop processed digit */
    }

    return result;
}
