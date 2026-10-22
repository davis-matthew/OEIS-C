/*
 * A379012  a(n) = A034444(n) if n is powerful (A001694), and 0 otherwise.
 * https://oeis.org/A379012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^ω(n) if n is powerful (all prime exponents ≥ 2),
 *            otherwise 0. ω(n) = number of distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* treat n as 64‑bit unsigned for factorisation */
    uint64_t m = (uint64_t)n;
    int distinct = 0;
    uint64_t d = 2;
    /* trial division up to sqrt(m) – use m/d to avoid overflow */
    while (d <= m / d)
    {
        int exp = 0;
        while (m % d == 0)
        {
            m /= d;
            exp++;
        }
        if (exp > 0)
        {
            if (exp == 1)          /* exponent 1 → not powerful */
                return 0;
            distinct++;            /* distinct prime found */
        }
        d++;
    }
    /* any remaining factor (m > 1) would have exponent 1 */
    if (m > 1)
        return 0;
    return pow2((idx_t)distinct);
}
