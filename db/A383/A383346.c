/*
 * A383346  Representation of n in rational base 3/2.
 * https://oeis.org/A383346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal integer whose digits are the base–3/2
 *            representation of n (digits 0,1,2)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n;   /* working value */
    seq_t result = 0;     /* the output decimal number */
    seq_t place = 1;      /* 10^k for the current digit position */

    if (n == 0)  /* a(0) = 0 */
        return 0;

    /* Compute the base-3/2 digits from least significant upwards. */
    while (r != 0)
    {
        seq_t d = (r * (seq_t)2) % (seq_t)3;      /* least significant digit */
        result = result + d * place;              /* accumulate decimal value */
        place  = place * (seq_t)10;               /* move to next decimal place */
        r = (r * (seq_t)2 - d) / (seq_t)3;       /* integer carry to the next digit */
    }
    return result;
}
