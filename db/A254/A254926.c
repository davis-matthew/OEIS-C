/*
 * A254926  There are a(n) numbers m such that 1 <= m <= n and gcd(m,n) is
 *          cubefree.
 * https://oeis.org/A254926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = number of integers m (1 ≤ m ≤ n) such that gcd(m,n) is
 *            cubefree. Equivalent multiplicative definition: a(p^e) = p^e if
 *            e < 3, a(p^e) = p^e – p^(e-3) for e ≥ 3
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint(idx_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for zero, but harmless if called */

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t term;
            if (e < 3)
                term = pow_uint(p, e);
            else {
                seq_t pe  = pow_uint(p, e);
                seq_t pe3 = pow_uint(p, e - 3);
                term = pe - pe3;
            }
            result *= term;
        }
    }

    if (temp > 1)   /* remaining prime factor with exponent 1 */
        result *= (seq_t)temp;

    return result;
}
