/*
 * A337454  a(n) is the number of divisors of n such that the ratio (the
 *          number of nonnegative m < n such that m^d == m (mod n))/(the
 *          number of nonnegative m < n such that -m^d == m (mod n)) is also a
 *          divisor of n.
 * https://oeis.org/A337454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d | n : (#{m < n : m^d ≡ m (mod n)} / #{m < n : -m^d ≡
 *            m (mod n)}) is a divisor of n}
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 112 terms through n = 4772 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary modular exponentiation using 128‑bit multiplication */
#include <stdint.h>
#include "../utils/powmod_u64.h"

/* Test whether divisor d satisfies the condition for given n */
static inline uint8_t divisor_ok(uint64_t d, uint64_t n)
{
    uint64_t count1 = 0, count2 = 0;
    for (uint64_t m = 0; m < n; ++m)
    {
        uint64_t p = powmod_u64(m, d, n);
        if (p == m) ++count1;
        if ((p + m) % n == 0) ++count2;
    }
    if (count2 == 0) return 0;
    if (count1 % count2 != 0) return 0;
    uint64_t ratio = count1 / count2;
    if (n % ratio != 0) return 0;
    return 1;
}

/* Main sequence function as required */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;   /* convert to native integer for loops */
    seq_t result = 0;

    for (uint64_t i = 1; i * i <= nn; ++i)
    {
        if (nn % i == 0)
        {
            if (divisor_ok(i, nn)) ++result;
            if (i != nn / i && divisor_ok(nn / i, nn)) ++result;
        }
    }
    return result;
}
