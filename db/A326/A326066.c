/*
 * A326066  a(n) = sigma(n) - sigma(A032742(n)), where A032742 gives the
 *          largest proper divisor of n.
 * https://oeis.org/A326066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) – sigma(largest proper divisor of n)
 * Range:     u8 n <= 167, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses trial division to compute the sum of divisors
 * sigma(n).  It obtains the smallest prime factor on the fly to find the
 * largest proper divisor.  No external library functions are required,
 * so the code compiles with any of the allowed seq_t/idx_t type
 * configurations. */

#include <stdint.h>

/* Compute σ(n) – the sum of all positive divisors of n.  Uses
 * trial division up to √n.  Works for any idx_t value that fits in
 * idx_t and returns a seq_t that can hold the result. */
static seq_t sigma_value(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* σ(1) = 1 */

    seq_t result = (seq_t)1;     /* product of sum–of–powers blocks */
    idx_t temp   = n;

    for (idx_t p = 2; p <= temp / p; ++p)     /* p*p <= temp */
    {
        if (temp % p != 0)
            continue;

        /* Compute geometric series 1 + p + p² + … + p^exp */
        seq_t term   = (seq_t)1;
        seq_t p_pow  = (seq_t)1;
        while (temp % p == 0)
        {
            temp   /= p;
            p_pow  *= (seq_t)p;
            term   += p_pow;
        }
        result *= term;
        if (temp == 1)          /* all prime factors processed */
            break;
    }

    /* If a prime factor > √n remains, multiply its contribution
     * 1 + that prime. */
    if (temp > 1)
        result *= ((seq_t)1 + (seq_t)temp);

    return result;
}

/* Return a(n) = σ(n) – σ(largest proper divisor of n). */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;        /* definition a(1)=0 */

    /* Find smallest prime factor */
    idx_t spf = 0;
    for (idx_t p = 2; p <= n / p; ++p)  /* p*p <= n */
    {
        if (n % p == 0)
        {
            spf = p;
            break;
        }
    }
    if (spf == 0)                /* n is prime */
        spf = n;

    /* Largest proper divisor */
    idx_t lpd = n / spf;

    /* Compute the two divisor sums */
    seq_t sigma_n   = sigma_value(n);
    seq_t sigma_lpd = sigma_value(lpd);

    return sigma_n - sigma_lpd;
}
