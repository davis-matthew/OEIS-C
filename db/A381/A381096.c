/*
 * A381096  Number of k <= n such that k is neither coprime to n and rad(k) !=
 *          rad(n), where rad = A007947.
 * https://oeis.org/A381096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - phi(n) - tau(n/rad(n)) * * phi(n) = n * Π_{p | n} (1
 *            - 1/p) * tau(n/rad(n)) = product of the exponents in the prime
 *            factorisation of n * * The function factors n by trial division,
 *            computes phi and the * product of exponents (tau), and returns
 *            a(n) = n - phi - tau
 * Range:     u8 n <= 290, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    seq_t sn = (seq_t)n;
    seq_t phi = sn;
    seq_t tau = 1;

    idx_t tmp = n;
    /* Factorisation, trial division */
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            /* update phi: phi = phi / p * (p - 1) */
            phi = phi / p * (p - 1);
            /* tau is product of exponents */
            tau = tau * (seq_t)exp;
        }
    }

    /* Remaining prime factor (if any) has exponent 1 and does not change tau */
    if (tmp > 1)
    {
        /* phi = phi / tmp * (tmp - 1) */
        phi = phi / tmp * (tmp - 1);
    }

    return sn - phi - tau;
}
