/*
 * A332209  Starting from sigma(n), number of halving and tripling steps to
 *          reach 1 in '3x+1' problem, or -1 if 1 is never reached.
 * https://oeis.org/A332209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total stopping time of the Collatz sequence starting at
 *            σ(n), the sum of the divisors of n. a(1) = 0, a(2) = 7, a(3) =
 *            2, …
 * Range:     u8 n <= 48, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1. Compute σ(n) by trial division. */
    uint64_t m = (uint64_t)n;
    seq_t sigma = (seq_t)1;

    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* term = (p^(e+1)-1)/(p-1) */
            seq_t p_pow = (seq_t)1;
            for (int k = 0; k < e + 1; ++k) {
                p_pow *= (seq_t)p;
            }
            seq_t term = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= term;
        }
    }
    if (m > 1) {            /* remaining prime factor */
        seq_t p = (seq_t)m;
        seq_t term = p + (seq_t)1;   /* (p^2-1)/(p-1) = p+1 */
        sigma *= term;
    }

    /* 2. Collatz total stopping time of sigma */
    uint64_t x = (uint64_t)sigma;
    uint64_t steps = 0;
    while (x != 1ULL) {
        ++steps;
        if ((x & 1ULL) == 0ULL)
            x >>= 1;
        else
            x = x * 3ULL + 1ULL;
    }
    return (seq_t)steps;
}
