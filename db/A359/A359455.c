/*
 * A359455  Expansion of Sum_{k>0} x^k / (1 - x^(k^3)).
 * https://oeis.org/A359455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that d^3 | (n − d)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 gracefully, though calls start at offset 1. */
    if (n == 0) return 0;

    seq_t count = 0;
    /* Enumerate divisors up to sqrt(n). */
    for (idx_t d = 1; (uint64_t)d * (uint64_t)d <= n; ++d)
    {
        if (n % d != 0) continue;

        /* First divisor d */
        seq_t sd = (seq_t)d;
        seq_t cube = sd * sd * sd;                 /* d^3 */
        seq_t rem  = ((seq_t)n - sd) % cube;
        if (rem == 0) ++count;

        /* Complementary divisor n/d, if distinct. */
        idx_t d2 = n / d;
        if (d2 != d)
        {
            seq_t sd2 = (seq_t)d2;
            seq_t cube2 = sd2 * sd2 * sd2;          /* (n/d)^3 */
            seq_t rem2  = ((seq_t)n - sd2) % cube2;
            if (rem2 == 0) ++count;
        }
    }

    return count;
}
