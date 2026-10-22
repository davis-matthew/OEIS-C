/*
 * A385638  Number of nonnegative s < n such that (-s)^s == -s^s (mod n).
 * https://oeis.org/A385638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2) + |{ even s < n : n | 2·s^s }| s = 0 works
 *            only for n = 1 or 2 (since 0^0 is defined as 1)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* count of odd s (< n) */
    seq_t count = (seq_t)(n / 2);

    /* test even s, including s = 0 */
    uint64_t modn = n;
    for (idx_t s = 0; s < n; s += 2U)
    {
        uint64_t pow_mod = mod_pow_uint64((uint64_t)s, (uint64_t)s, modn);
        /* check 2 * pow_mod ≡ 0 (mod n) */
        if ((2ULL % modn) * pow_mod % modn == 0ULL)
            count += 1;
    }
    return count;
}
