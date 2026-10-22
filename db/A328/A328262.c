/*
 * A328262  a(n) = a(n-1)*3/2, if noninteger then rounded to the nearest even
 *          integer, with a(1) = 1.
 * https://oeis.org/A328262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 55, u64 n <= 109,
 *            u128 n <= 219, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) = round(a(n-1) * 3/2) to the nearest
   even integer (rounding only occurs when a(n-1) is odd). */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;      /* undefined offset, safe return */
    seq_t cur = (seq_t)1;
    if (n == (idx_t)1) return cur;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* floor(3*cur/2) == cur + floor(cur/2) */
        seq_t f = cur + cur / (seq_t)2;

        /* Rounding is needed only when cur is odd (3*cur is odd). */
        if ((cur % (seq_t)2) != (seq_t)0)
        {
            /* If f is odd, add 1 to make it the nearest even integer. */
            if ((f % (seq_t)2) != (seq_t)0)
                f += (seq_t)1;
        }
        cur = f;
    }
    return cur;
}
