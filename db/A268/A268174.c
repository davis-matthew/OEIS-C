/*
 * A268174  Integers m such that m^(m+1) == 1 (modulo (m+2)).
 * https://oeis.org/A268174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer m such that m^(m+1) ≡ 1 (mod m+2)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 769 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t powmod_ull(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;        /* n is 1‑based */
    uint64_t found  = 0;
    for (uint64_t m = 1;; m += 2)         /* only odd m can work */
    {
        uint64_t p = m + 2;          /* m+2 must be odd and >2 */
        /* test: m^(m+1) ≡ 1 (mod m+2)  ⇔  m^(p-1) ≡ 1 (mod p) */
        if (powmod_ull(m, p - 1, p) == (seq_t)1)
        {
            ++found;
            if (found == target)
                return (seq_t)m;
        }
    }
}
