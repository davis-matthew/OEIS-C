/*
 * A384443  a(n) is the product of the prime digits of n; or 1 if n contains
 *          no prime digits.
 * https://oeis.org/A384443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of the prime digits of n; return 1 if there are
 *            none
 * Range:     u8 n <= 776, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t t = n;

    /* If n is 0 (outside the official range) treat as having no prime digits. */
    if (t == 0)
        return prod;           /* prod is 1 */

    while (t != 0)
    {
        unsigned d = t % 10u;    /* extract last decimal digit */

        switch (d)
        {
            case 2: case 3: case 5: case 7:
                prod *= (seq_t)d;   /* multiply if digit is prime */
                break;
        }

        t /= 10u;                 /* shift right by one decimal digit */
    }

    return prod;
}
