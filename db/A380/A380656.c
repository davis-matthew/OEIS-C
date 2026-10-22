/*
 * A380656  a(n) is the number of divisors d such that -d^n mod n = d.
 * https://oeis.org/A380656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors d of n such that (-d^n) mod n =
 *            d (equivalently, d^n + d is a multiple of n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
/* multiply modulo mod using a 64‑bit intermediate to avoid overflows
   on small integer types and to work with arbitrary‑precision types. */
{
    unsigned long long t =
        (unsigned long long)(a % mod) * (unsigned long long)(b % mod);
    return (seq_t)(t % (unsigned long long)mod);
}

static inline seq_t mod_pow_internal(seq_t base, idx_t exp, seq_t mod)
/* modular exponentiation via repeated squaring, uses mul_mod. */
{
    seq_t result = 1 % mod;
    seq_t cur    = base % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = mul_mod(result, cur, mod);
        cur = mul_mod(cur, cur, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, a(0) unused */
        return 0;

    seq_t count = 0;
    seq_t modN  = (seq_t)n;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        /* test divisor d1 (proper divisor) */
        if (d1 < n) {
            seq_t p = mod_pow_internal((seq_t)d1, n, modN);
            seq_t t = (modN - p) % modN;
            if (t == (seq_t)d1)
                ++count;
        }

        /* test complementary divisor d2, distinct and proper */
        if (d2 != d1 && d2 < n) {
            seq_t p = mod_pow_internal((seq_t)d2, n, modN);
            seq_t t = (modN - p) % modN;
            if (t == (seq_t)d2)
                ++count;
        }
    }

    return count;
}
