/*
 * A386776  Number of divisors d of n such that d^d == d^n (mod n).
 * https://oeis.org/A386776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #divisors d of n such that d^d ≡ d^n (mod n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast modular multiplication using 128‑bit intermediate */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* fast modular exponentiation, exponent fits in idx_t (= uint64_t) */
static uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1) result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* helper: test a single divisor */
static int check_divisor_uint64(uint64_t d, uint64_t n)
{
    uint64_t pd = mod_pow_uint64(d, d, n);
    uint64_t pn = mod_pow_uint64(d, n, n);
    return pd == pn;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, safe guard */
    if (n == 1) return 1;          /* only divisor 1 */

    seq_t count = 0;
    uint64_t N = (uint64_t)n;

    for (uint64_t i = 1; i * i <= N; ++i)
    {
        if (N % i == 0)
        {
            uint64_t d1 = i;
            uint64_t d2 = N / i;

            if (check_divisor_uint64(d1, N)) ++count;
            if (d1 != d2 && check_divisor_uint64(d2, N)) ++count;
        }
    }
    return count;
}
