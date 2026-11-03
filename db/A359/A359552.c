/*
 * A359552  Multiplicative with a(p^e) = 1 if e == 0 mod p, otherwise 0.
 * https://oeis.org/A359552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if for every prime p dividing n the exponent of p
 *            satisfies e ≡ 0 (mod p); otherwise 0
 * Range:     u8 n <= 110592, u16 n <= 110592, u32 n <= 110592,
 *            u64 n <= 110592, u128 n <= 110592, big any n
 * Validated: standard, pass, 124 terms through n = 9828 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    for (idx_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            idx_t exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            if (exp % p != 0)
                return (seq_t)0;
        }
    }
    if (n > 1)   /* remaining prime factor has exponent 1, which cannot satisfy the condition */
        return (seq_t)0;

    return (seq_t)1;
}
