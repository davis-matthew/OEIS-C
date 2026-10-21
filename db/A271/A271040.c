/*
 * A271040  Number of different 3 against 3 matches given n players.
 * https://oeis.org/A271040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)/72
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 6 there are no disjoint triples of size 3 */
    if (n < 6) return 0;

    /* Denominator 72 = 2^3 * 3^2.  We cancel powers of 2 and 3
       from the six consecutive numbers n, n-1, …, n-5 as we
       multiply them together.  This keeps intermediate results
       small enough for uint64_t output while still working with
       seq_t (which may be 128-bit or arbitrary precision). */
    seq_t rem2 = 3;            /* remaining 2 factors to cancel   */
    seq_t rem3 = 2;            /* remaining 3 factors to cancel   */
    seq_t res  = 1;            /* running product in seq_t form   */

    for (idx_t i = 0; i < 6; ++i)
    {
        seq_t m = (seq_t)(n - i);   /* current factor, cast to seq_t */

        /* Cancel powers of 2 as long as we still need them
           and the factor is even. */
        while (rem2 > 0 && (m & 1) == 0)
        {
            m >>= 1;   /* divide by 2 */
            rem2--;
        }

        /* Cancel powers of 3 similarly. */
        while (rem3 > 0 && m % 3 == 0)
        {
            m /= 3;    /* divide by 3 */
            rem3--;
        }

        /* Multiply the reduced factor into the result. */
        res *= m;
    }

    return res;
}
