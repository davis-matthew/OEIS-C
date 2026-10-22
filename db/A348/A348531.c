/*
 * A348531  Number of partitions of n into 3 parts where at least one of the
 *          parts divides the product of the other two.
 * https://oeis.org/A348531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered triples (a,b,c) with a<=b<=c, a+b+c =
 *            n, such that at least one part divides the product of the other
 *            two
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 there are no 3-part partitions. */
    if (n < 3) return (seq_t)0;

    idx_t count = 0;
    for (idx_t a = 1; a <= n / 3; ++a)
    {
        idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b)
        {
            idx_t c = n - a - b;   /* ensures c >= b */
            if (((b * c) % a) == 0 ||
                ((a * c) % b) == 0 ||
                ((a * b) % c) == 0)
            {
                ++count;
            }
        }
    }

    return (seq_t)count;
}
