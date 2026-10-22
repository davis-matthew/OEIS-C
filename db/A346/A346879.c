/*
 * A346879  Sum of the divisors, except the smallest and the largest, of the
 *          n-th odd number.
 * https://oeis.org/A346879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2*n-1) - 2*n (sum of interior divisors of the n-th
 *            odd number)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2*n-1 is the n-th odd number */
    idx_t m = 2 * n - 1;
    if (m <= 1)               /* m == 1 -> no interior divisors */
        return 0;

    /* compute σ(m) = product over prime powers of (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;
    idx_t remaining = m;

    for (idx_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0) {
            seq_t p_power = 1;
            seq_t term = 1;
            while (remaining % p == 0) {
                remaining /= p;
                p_power *= p;
                term += p_power;
            }
            sigma *= term;
        }
    }
    if (remaining > 1) {          /* remaining is a prime factor > sqrt(m) */
        sigma *= (1 + remaining);
    }

    /* a(n) = sigma(2*n-1) - 2*n  */
    seq_t two_n = (seq_t)n * 2;
    if (sigma <= two_n)          /* should only happen for primes */
        return 0;
    return sigma - two_n;
}
