/*
 * A255385  a(n) = sigma(n) + phi(n) - tau(n).
 * https://oeis.org/A255385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + phi(n) - tau(n) (OEIS A255385)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: trial‑division factorisation, accumulating sigma,
   Euler's totient and divisor count during the same loop. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Handle the trivial case n == 1 separately to avoid the loop. */
    if (n == 1)
        return 1;   /* sigma=1, phi=1, tau=1  → 1+1-1 = 1 */

    /* Use 64‑bit unsigned integers for intermediate arithmetic; the
       final result is cast back to seq_t.  Intermediate values never
       exceed 2^64 for the intended range of n (up to at least 10 000). */
    uint64_t m = (uint64_t)n;
    uint64_t sigma = 1;
    uint64_t phi_val = (uint64_t)n;
    uint64_t tau = 1;

    /* Trial division factorisation.  The loop variable p stays inside
       the 64‑bit range, and p*p <= m guarantees termination. */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            /* Count exponent of prime p in n. */
            uint64_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                exp++;
            }

            /* Multiply sigma by the sum of p^0 + p^1 + … + p^exp. */
            /*  p^(exp+1) = p * p^exp . */
            uint64_t p_pow_exp1 = 1;
            for (uint64_t i = 0; i < exp + 1; ++i)
                p_pow_exp1 *= p;
            uint64_t sigma_term = (p_pow_exp1 - 1) / (p - 1);
            sigma *= sigma_term;

            /* Update the divisor count τ(n). */
            tau *= (exp + 1);

            /* Update Euler's totient φ(n).  φ(n) = φ(n) * (p‑1)/p. */
            phi_val = phi_val / p * (p - 1);
        }
    }

    /* If m > 1 at this point, then m itself is a prime factor with
       exponent 1.  Handle the remaining prime separately. */
    if (m > 1)
    {
        uint64_t p = m;
        /* Sum of powers: 1 + p. */
        uint64_t sigma_term = 1 + p;
        sigma *= sigma_term;

        tau *= 2;                 /* exponent 1 → +1 */

        phi_val = phi_val / p * (p - 1);
    }

    uint64_t result = sigma + phi_val - tau;
    return (seq_t)result;
}
