/*
 * A328854  Dirichlet g.f.: Product_{p prime} 1 / (1 - 2 * p^(-s))^2.
 * https://oeis.org/A328854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \prod_{p^k\parallel n} 2^k (k+1)
 * Range:     u8 n <= 47, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A328854: a(n) = 2^bigomega(n) * tau(n) */
seq_t a(idx_t n)
{
    /* a(1) = 1 by construction */
    seq_t result = 1;
    idx_t tmp = n;

    if (tmp == 0)   /* safety for out‑of‑range calls */
        return 0;

    /* trial division for all factors of tmp */
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            seq_t pow2 = ((seq_t)1) << exp;          /* 2^exp */
            seq_t mult = (seq_t)(exp + 1);          /* k+1 */
            result = result * pow2 * mult;
        }
    }

    /* any remaining prime factor of exponent 1 */
    if (tmp > 1)
    {
        seq_t pow2 = ((seq_t)1) << 1;              /* 2^1 */
        seq_t mult = (seq_t)2;                    /* 1+1 */
        result = result * pow2 * mult;
    }

    return result;
}
