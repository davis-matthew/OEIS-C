/*
 * A291784  a(n) = (psi(n) + phi(n))/2.
 * https://oeis.org/A291784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (psi(n) + phi(n))/2
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where psi is the Dedekind psi function
 *   psi(n) = n * ∏_{p|n} (1 + 1/p)
 *   and phi is Euler's totient function.
 *   The function works for any idx_t n >= 1.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the problem but harmless */

    seq_t psi  = 1;                 /* Dedekind psi */
    seq_t phi  = 1;                 /* Euler totient */
    idx_t temp = n;

    /* factorize n by trial division */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            seq_t pk   = 1;       /* p^k  */
            /* count exponent and build p^k */
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
                pk *= (seq_t)p;
            }
            /* p^(k-1) = pk / p      (works even for exp == 1) */
            seq_t pk_minus1 = pk / (seq_t)p;

            /* multiplicative terms for psi and phi */
            seq_t term_psi = pk + pk_minus1;   /* p^k + p^(k-1) */
            seq_t term_phi = pk - pk_minus1;   /* p^k - p^(k-1) */

            psi *= term_psi;
            phi *= term_phi;
        }
    }

    /* remaining prime factor larger than sqrt(original n) */
    if (temp > 1)
    {
        seq_t pk   = (seq_t)temp;       /* prime^1 */
        seq_t pk_minus1 = 1;             /* prime^(1-1) */
        seq_t term_psi = pk + pk_minus1; /* temp + 1 */
        seq_t term_phi = pk - pk_minus1; /* temp - 1 */
        psi *= term_psi;
        phi *= term_phi;
    }

    /* final result: (psi + phi) / 2  (always integer) */
    return (psi + phi) / 2;
}
