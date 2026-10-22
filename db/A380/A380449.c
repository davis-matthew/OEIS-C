/*
 * A380449  Sum of the divisors of n plus the number of nonprime divisors of
 *          n.
 * https://oeis.org/A380449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + tau(n) - omega(n)
 * Range:     u8 n <= 48, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(n)  = sum of all divisors */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle n >= 1; no special treatment needed for n == 0 */
    uint64_t temp = (uint64_t)n;
    seq_t sigma = 1;      /* product of sigma contributions for each prime power */
    seq_t tau   = 1;      /* product of (exponent+1) for each prime power */
    seq_t omega = 0;      /* count of distinct prime factors */

    /* Special case for factor 2 */
    int e = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        ++omega;
        seq_t pow = 1;
        for (int i = 0; i < e + 1; ++i)
            pow *= (seq_t)2ULL;      /* 2^(e+1) */
        seq_t sigma_contrib = pow - 1;          /* (2^(e+1)-1)/(2-1) */
        sigma *= sigma_contrib;
        tau   *= (seq_t)(e + 1);
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            ++omega;
            seq_t pow = 1;
            for (int i = 0; i < e + 1; ++i)
                pow *= (seq_t)p;      /* p^(e+1) */
            seq_t sigma_contrib = (pow - 1) / ((seq_t)p - 1);
            sigma *= sigma_contrib;
            tau   *= (seq_t)(e + 1);
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        ++omega;
        seq_t sigma_contrib = (seq_t)temp + 1;     /* p^1 + 1 */
        sigma *= sigma_contrib;
        tau   *= (seq_t)2;
    }

    return sigma + tau - omega;
}
