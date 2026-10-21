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
 * Range:     u8 n <= 303, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   phi_and_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_and_tau.h"

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
