/*
 * A351307  Sum of the squares of the square divisors of n.
 * https://oeis.org/A351307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2|n} (d^2)^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is multiplicative; for a prime power p^e the factor is
   sum_{f=0}^{floor(e/2)} p^(4f).  The implementation
   factorises n with trial division and computes that product. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, return 0 */
    seq_t result = 1;       /* multiplicative identity */

    idx_t m = n;
    /* trial division */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p != 0)
            continue;

        idx_t exp = 0;
        while (m % p == 0)
        {
            m /= p;
            ++exp;
        }

        idx_t fmax = exp / 2;        /* floor(e/2) */
        seq_t term = 0;
        seq_t p4pow = 1;             /* p^(4f) for f=0 initially 1 */
        for (idx_t f = 0; f <= fmax; ++f)
        {
            term += p4pow;
            p4pow *= (seq_t)p * p * p * p;  /* multiply by p^4 */
        }
        result *= term;
    }

    /* if m > 1 at this point, it is a prime factor > sqrt(original n) */
    if (m > 1)
    {
        /* exponent = 1 => fmax = 0 => term = 1, no change to result */
        /* no action needed */
    }

    return result;
}
