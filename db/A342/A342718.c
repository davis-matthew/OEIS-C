/*
 * A342718  a(1) = 0; for n >= 2, a(n) is the number of iterations needed for
 *          the map x -> A000203(x)/A000005(x) to reach a number that is not
 *          an integer, when starting from x = n.
 * https://oeis.org/A342718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations (σ(x)/τ(x) until non‑integer), where
 *            σ(x) is the sum of divisors and τ(x) the divisor count; a(1) = 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes σ(n)/τ(n) for an n that fits in uint64_t. The result is
   stored in *sout (σ) and *tout (τ). */
static void sigma_tau_uint64(uint64_t n, seq_t *sout, seq_t *tout)
{
    uint64_t sigma = 1;
    uint64_t tau   = 1;
    uint64_t m = n;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            /* sum of powers of p up to exp: 1 + p + p^2 + ... + p^exp */
            uint64_t sum_powers = 1;
            uint64_t power      = 1;
            for (uint64_t i = 0; i < exp; ++i)
            {
                power *= p;
                sum_powers += power;
            }
            sigma *= sum_powers;
            tau   *= (exp + 1);
        }
    }
    if (m > 1)           /* remaining prime factor */
    {
        sigma *= (1 + m);
        tau   *= 2;
    }
    *sout = (seq_t)sigma;
    *tout = (seq_t)tau;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    seq_t cur = (seq_t)n;  /* current value as an arbitrary‑precision int */
    int count = 0;

    while (1)
    {
        /* The current value will never overflow uint64_t for the
           ranges of n we expect to handle (≤ 10⁴ at least).  */
        uint64_t cur64 = (uint64_t)cur;
        seq_t sigma, tau;
        sigma_tau_uint64(cur64, &sigma, &tau);

        ++count;
        if (sigma % tau != 0)
            break;

        cur = sigma / tau;
    }

    return (seq_t)count;
}
