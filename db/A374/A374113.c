/*
 * A374113  a(n) = 1 if A113177(n) and A276085(n) are both even, otherwise 0,
 *          where A113177 and A276085 are fully additive with a(p) =
 *          Fibonacci(p) and a(p) = p#/p, respectively.
 * https://oeis.org/A374113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9213, u16 n <= 9213, u32 n <= 9213, u64 n <= 9213,
 *            u128 n <= 9213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A374113: a(n) = 1 if the 2-adic valuation of n is even,
 *      and the parity of the total number of prime factors (Ω(n))
 *      equals the parity of the 3-adic valuation of n; otherwise 0.
 * This implementation uses trial division; it is fast for n up to at least 10,000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safely returns 0 */

    /* 2-adic valuation parity */
    int v2_parity = 0;
    uint64_t m = n;
    while ((m & 1) == 0) {
        v2_parity ^= 1;          /* toggle parity for each factor of 2 */
        m >>= 1;
    }

    /* 3-adic valuation parity and Ω parity */
    int v3_parity = 0;
    int omega_parity = v2_parity; /* total parity starts with parity of 2-factors */
    // m currently has no factor 2
    while (m % 3 == 0) {
        v3_parity ^= 1;
        omega_parity ^= 1;      /* toggle for each factor of 3 */
        m /= 3;
    }

    /* Factor remaining part (>1) by trial division starting from 5 */
    for (uint64_t p = 5; p * p <= m; p += 6) {
        while (m % p == 0) {
            omega_parity ^= 1;
            m /= p;
        }
        while (m % (p + 2) == 0) {
            omega_parity ^= 1;
            m /= (p + 2);
        }
    }
    if (m > 1) {            /* remaining prime factor */
        omega_parity ^= 1;
    }

    /* Check conditions */
    int result = (v2_parity == 0 && omega_parity == v3_parity) ? 1 : 0;
    return (seq_t)result;
}
