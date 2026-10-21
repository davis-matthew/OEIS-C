/*
 * A255361  Number of ways n can be represented as x*y+x+y where x>=y>1.
 * https://oeis.org/A255361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as x*y+x+y with x ≥ y > 1.
 *            Using the identity n+1 = (x+1)(y+1). For n even:
 *            a(n)=⌊(d(n+1)–1)/2⌋, for n odd > 1: a(n)=⌊(d(n+1)–3)/2⌋, where
 *            d(m) is the divisor count of m
 * Range:     u8 n <= 3300, u16 n <= 3300, u32 n <= 3300, u64 n <= 3300,
 *            u128 n <= 3300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset and trivial cases */
    if (n == 0 || n == 1)
        return 0;

    /* Count divisors of n+1 */
    idx_t nplus = n + 1;          /* n+1 fits in idx_t (uint64_t) */
    idx_t div_cnt = 0;            /* divisor count, also fits in idx_t */

    for (idx_t i = 1; i <= nplus / i; ++i)
    {
        if (nplus % i == 0)
        {
            ++div_cnt;                     /* divisor i */
            if (i != nplus / i)
                ++div_cnt;                 /* divisor nplus/i */
        }
    }

    seq_t d = (seq_t)div_cnt;      /* d(n+1) */

    if ((n & 1) == 0)                     /* even n */
        return (d - 1) / 2;
    else                                   /* odd n > 1 */
        return (d - 3) / 2;
}
