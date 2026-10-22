/*
 * A318515  a(n) = n AND A032742(n), where AND is bitwise-and (A004198) and
 *          A032742 = the largest proper divisor of n.
 * https://oeis.org/A318515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n AND A032742(n) where A032742(n) is the largest proper
 *            divisor of n (for n = 1, it is defined to be 1)
 * Range:     u8 n <= 767, u16 n <= 8790, u32 n <= 8790, u64 n <= 8790,
 *            u128 n <= 8790, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external headers are required; all types are provided by the build. */
seq_t a(idx_t n)
{
    /* Handle the special case n = 0 (outside the offset); return 0. */
    if (n == 0)
        return (seq_t)0;

    /* For n = 1, the largest proper divisor is defined as 1. */
    if (n == 1)
        return (seq_t)1;

    /* Compute the smallest prime factor of n. */
    idx_t sp = 0;
    for (idx_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            sp = p;
            break;
        }
    }
    if (sp == 0)
        sp = n; /* n is prime */

    /* Largest proper divisor = n / sp. */
    seq_t lpd = (seq_t)(n / sp);

    /* Return bitwise AND of n and its largest proper divisor. */
    return (seq_t)n & lpd;
}
