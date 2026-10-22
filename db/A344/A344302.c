/*
 * A344302  Number of cyclic subgroups of the group (C_n)^6, where C_n is the
 *          cyclic group of order n.
 * https://oeis.org/A344302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Number of cyclic subgroups of (C_n)^6. a(n) is
 *            multiplicative. For n = ∏ p_i^{e_i}: a(n) = ∏ [ 1 + (1 + p_i +
 *            p_i^2 + p_i^3 + p_i^4 + p_i^5) * (1 + p_i^5 + p_i^10 + … +
 *            p_i^{5(e_i-1)}) ]
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 69, u64 n <= 5459,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a single term for a prime power p^e.
   The result is 1 + sum_{k=0..5} p^k  *  (sum_{j=0..e-1} p^{5j}) */
static seq_t prime_power_term(seq_t p, idx_t e)
{
    seq_t sum6 = 1;      /* 1 */
    seq_t term  = p;     /* p^1 */
    for (int i = 1; i < 6; ++i) {   /* add p^1 .. p^5 */
        sum6 += term;
        term *= p;                     /* term = p^{i+1} */
    }

    /* compute p^5 once for the second geometric series */
    seq_t p5 = 1;
    for (int i = 0; i < 5; ++i) {
        p5 *= p;                     /* p5 = p^5 */
    }

    seq_t sum5e = 1;          /* first term of 1 + p^5 + p^10 + ... */
    seq_t term5 = 1;          /* p^{5j} term */
    for (idx_t i = 1; i < e; ++i) {
        term5 *= p5;          /* p^{5i} */
        sum5e += term5;
    }

    return 1 + sum6 * sum5e;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; n=0 is unused */
        return 0;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            seq_t term = prime_power_term((seq_t)p, e);
            result *= term;
        }
    }

    if (temp > 1)        /* residual prime factor with exponent 1 */
    {
        seq_t term = prime_power_term((seq_t)temp, 1);
        result *= term;
    }

    return result;
}
