/*
 * A392677  a(n) = Sum_{i=1..n} i^2*(-1)^ceiling(sqrt(i)).
 * https://oeis.org/A392677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{i=1..n} i^2 * (-1)^ceil(sqrt(i))
 * Range:     i8 n <= 7, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The sign is constant for the range of i that belong to the same "
 *   ceil(sqrt(i)).  For k = 1,2,3,... the interval is: */
 /*   i in [(k-1)^2+1 .. k^2] with sign (-1)^k.               */
/*   The loop below walks through the integers 1..n, flipping the sign
 *   whenever we pass a perfect square.  All arithmetic is done with
 *   the type seq_t to keep the result exact.                            */
seq_t a(idx_t n)
{
    seq_t sum         = 0;
    seq_t sign        = -1;          /* sign for current block */
    idx_t k           = 1;           /* current block number (ceil sqrt) */
    idx_t limit       = 1 * 1;       /* upper bound for current block */
    idx_t i;

    for (i = 1; i <= n; ++i)
    {
        /* When i has exceeded the current block's bound,
           move to the next block and flip the sign. */
        if (i > limit)
        {
            ++k;
            sign = -sign;
            limit = k * k;
        }

        /* Add i^2 * sign to the running total. */
        seq_t ii = (seq_t)i;
        sum += sign * ii * ii;
    }

    return sum;
}
