/*
 * A292269  If n is 1 or a prime, then a(n) = 1, otherwise a(n) = the third
 *          smallest divisor of n.
 * https://oeis.org/A292269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is 1 or prime; otherwise the third smallest
 *            divisor of n
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 1 */
    if (n == 1)
        return 1;

    /* Find the smallest divisor > 1.  If none, n is prime. */
    idx_t p = 0;                       /* smallest divisor of n > 1 */
    for (idx_t i = 2; i <= n / i; ++i) /* up to sqrt(n) */
    {
        if (n % i == 0)
        {
            p = i;
            break;
        }
    }
    if (p == 0)        /* n is prime */
        return 1;

    /* Find the smallest divisor of n that is greater than p.
       We examine both the divisor itself and its complementary divisor. */
    seq_t third = 0;                    /* value of the third smallest divisor */
    for (idx_t i = 2; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t a_div = i;            /* divisor i */
            idx_t b_div = n / i;        /* complementary divisor n/i */

            if (a_div > p)
            {
                seq_t cand = (seq_t)a_div;
                if (third == 0 || cand < third)
                    third = cand;
            }
            if (b_div > p && b_div != a_div)
            {
                seq_t cand = (seq_t)b_div;
                if (third == 0 || cand < third)
                    third = cand;
            }
        }
    }

    /* If no divisor greater than p was found, n is a prime power.
       Then the third divisor is p^2 (which is <= n). */
    if (third == 0)
        third = (seq_t)p * (seq_t)p;

    return third;
}
