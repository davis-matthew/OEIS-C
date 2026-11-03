/*
 * A264441  Length of row n of the irregular triangle A133995 (positive
 *          integers <= n which are neither divisors of n nor coprime to n).
 * https://oeis.org/A264441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(1, n - φ(n) - τ(n) + 1) where φ(n) is Euler's totient
 *            function and τ(n) is the number of divisors of n. The
 *            implementation factorizes n by trial division, which is fast
 *            enough for n ≤ 10 000
 * Range:     u8 n <= 303, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 87 terms through n = 9623 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper that returns φ(n) and τ(n) for a given n.
   Both results are written to the supplied pointers. */
static void phi_and_tau(uint64_t n, seq_t *phi, seq_t *tau)
{
    if (n == 0)
    {
        *phi = 0;
        *tau = 0;
        return;
    }

    seq_t phi_val = (seq_t)n;
    seq_t tau_val = 1;
    uint64_t m = n;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                exp++;
            }
            phi_val = phi_val / p * (p - 1);
            tau_val *= (exp + 1);
        }
    }

    if (m > 1)            /* remaining prime factor */
    {
        phi_val = phi_val / m * (m - 1);
        tau_val *= 2;
    }

    *phi = phi_val;
    *tau = tau_val;
}

/* Sequence a(n): Length of row n of irregular triangle A133995
   (positive integers ≤ n which are neither divisors of n nor coprime to n). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; behaviour below is irrelevant */
        return 0;

    seq_t phi_val, tau_val;
    phi_and_tau((uint64_t)n, &phi_val, &tau_val);

    /* Compute n - φ(n) - τ(n) + 1 with signed arithmetic to avoid underflow. */
    long long tmp = (long long)n - (long long)phi_val - (long long)tau_val + 1;
    if (tmp < 1)
        tmp = 1;

    return (seq_t)tmp;
}
