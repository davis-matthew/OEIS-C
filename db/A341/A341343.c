/*
 * A341343  Dirichlet g.f.: Sum_{n>=1} a(n)/n^s = (zeta(s))^3 / (zeta(3*s))^2.
 * https://oeis.org/A341343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} S[min(e,4)] where S = [1,3,6,8,9]
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (S gives the cumulative sums of the trinomial coefficients of (1+x+x²)²) */

static const seq_t PREFIX[5] = { (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)8, (seq_t)9 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; should not be called for 0 */
    seq_t result = 1;
    idx_t tmp = n;

    /* factorisation by trial division (fast enough for n ≤ 10 000) */
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            result *= PREFIX[exp <= 4 ? exp : 4];
        }
    }

    /* remaining large prime factor, if any */
    if (tmp > 1)
        result *= PREFIX[1];   /* exponent 1 ⇒ prefix[1] = 3 */

    return result;
}
