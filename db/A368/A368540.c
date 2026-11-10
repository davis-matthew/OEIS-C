/*
 * A368540  The smallest unitary divisor d of n such that n/d is a term of
 *          A138302.
 * https://oeis.org/A368540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n / A367168(n); multiplicative with a(p^e) = p^(e -
 *            A048298(e)), i.e. p^e if e is not a power of 2, else 1
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 143 terms through n = 9209 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A368540_pow2(idx_t e)
{
    return e > 0 && (e & (e - 1)) == 0;
}

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    if (n < 2)
        return res;
    for (idx_t p = 2; p <= n / p; p += (p == 2 ? 1 : 2))
    {
        if (n % p == 0)
        {
            idx_t e = 0;
            seq_t pe = (seq_t)1;
            while (n % p == 0)
            {
                n /= p;
                ++e;
                pe *= (seq_t)p;
            }
            if (!A368540_pow2(e))
                res *= pe;
        }
    }
    /* remaining prime has exponent 1, a power of 2, so contributes 1 */
    return res;
}
