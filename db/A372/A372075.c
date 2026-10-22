/*
 * A372075  a(1) = 1; thereafter a(n+1) is obtained by prepending the
 *          digit-sum of a(n) to a(n).
 * https://oeis.org/A372075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;  for n > 1,
   a(n) = digit_sum(a(n-1)) * 10^digit_len(a(n-1)) + a(n-1) */

/* Computes the OEIS sequence A372075. */
/* The function works for any unsigned integer type seq_t (uint8_t … uint64_t, unsigned __int128)
   and any unsigned index type idx_t (typically uint64_t). */
seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 1.  For other inputs we return 0. */
    if (n < (idx_t)1)
        return (seq_t)0;

    seq_t current = (seq_t)1;          /* a(1) = 1 */

    /* Compute a(2) … a(n).  For each step we need the sum of decimal digits
       and the number of decimal digits of the previous value. */
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t temp = current;

        seq_t sum = (seq_t)0;        /* digit sum of current */
        idx_t  len = (idx_t)0;       /* number of decimal digits of current */

        while (temp > (seq_t)0)
        {
            sum += temp % (seq_t)10; /* last decimal digit */
            len++;                   /* count a digit */
            temp /= (seq_t)10;       /* drop last digit */
        }

        /* Compute 10^len.  len fits into idx_t, but we multiply using seq_t. */
        seq_t pow10 = (seq_t)1;
        for (idx_t j = (idx_t)0; j < len; ++j)
            pow10 *= (seq_t)10;

        /* Prepend the digit sum: concatenate sum in front of current. */
        current = sum * pow10 + current;
    }

    return current;
}
