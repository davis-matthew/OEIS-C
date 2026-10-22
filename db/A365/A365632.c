/*
 * A365632  The number of divisors of n that are terms of A072873.
 * https://oeis.org/A365632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (1 + floor(e/p))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    uint64_t m = (uint64_t)n;           /* copy to mutable local */

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* floor(e/p) is zero for e < p; cast to seq_t safely */
            seq_t term = (seq_t)(1 + e / p);
            res *= term;
        }
    }

    /* If a prime factor larger than sqrt(original n) remains, its exponent e=1,
       so floor(1/p)=0 and (1+0)=1 – multiplying by 1 is harmless.          */
    if (m > 1) {
        /* term would be 1, no change to res, but keep explicit multiplicative form */
        seq_t term = (seq_t)(1 + 1 / m);  /* 1/m == 0 for m >= 2 */
        res *= term;
    }

    return res;
}
