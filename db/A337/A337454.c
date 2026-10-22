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
 * Helpers:   divisor_ok, powmod_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary modular exponentiation using 128‑bit multiplication */
#include <stdint.h>
#include "../utils/divisor_ok.h"
#include "../utils/powmod_uint.h"

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
