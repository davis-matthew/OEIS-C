/*
 * A258331  Sum of the cubes of the divisors of n^3.
 * https://oeis.org/A258331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_3(n^3) = ∏_{p^e‖n} (1 + p^3 + p^6 + … + p^(9e))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 137,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, but guard against 0 */
    seq_t result = 1;

    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p != 0) continue;
        int e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }
        seq_t p3 = (seq_t)p * p * p;      /* p^3, fits in seq_t for n ≤ 10000 */
        seq_t term = 1;                   /* start with 1 (p^(0)) */
        seq_t power = 1;
        for (int i = 0; i < 3 * e; ++i)
        {
            power *= p3;
            term += power;
        }
        result *= term;
    }

    /* Remaining prime factor > sqrt(n) */
    if (tmp > 1)
    {
        uint64_t p = tmp;
        int e = 1;
        seq_t p3 = (seq_t)p * p * p;
        seq_t term = 1;
        seq_t power = 1;
        for (int i = 0; i < 3 * e; ++i)
        {
            power *= p3;
            term += power;
        }
        result *= term;
    }

    return result;
}
