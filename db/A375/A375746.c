/*
 * A375746  The alternating sum of sequentially decreasing moduli for every
 *          positive integer.
 * https://oeis.org/A375746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * (n mod k)
 * Range:     i8 n <= 143, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 just return 0; the sequence is defined for n >= 1. */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Remainder of n divided by k.  n%k fits in idx_t. */
        seq_t r = (seq_t)(n % k);

        /* Add or subtract r depending on parity of (n-k). */
        if ((n - k) & (idx_t)1)
            sum -= r;   /* odd -> negative sign */
        else
            sum += r;   /* even -> positive sign */
    }
    return sum;
}
