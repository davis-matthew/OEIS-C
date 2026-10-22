/*
 * A391196  a(n) = (2^(n-2) * (n-2)! + floor(n/2)) mod n for n > 1.
 * https://oeis.org/A391196
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n-2) * (n-2)! + floor(n/2)) mod n for n > 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: modular multiplication using 64‑bit intermediate to avoid overflow */
static inline seq_t modmul(seq_t x, seq_t y, seq_t mod)
{
    return (seq_t)((unsigned long long)x * (unsigned long long)y % (unsigned long long)mod);
}

/* Helper: modular exponentiation (binary powering) */
static inline seq_t modpow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t base_mod = base % mod;
    while (exp) {
        if (exp & 1)
            result = modmul(result, base_mod, mod);
        base_mod = modmul(base_mod, base_mod, mod);
        exp >>= 1;
    }
    return result;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* not needed for proper OEIS range */

    seq_t mod = n;                /* n fits in seq_t for n ≤ 10000 */
    seq_t floor_half = (seq_t)(n / 2);   /* floor(n/2), using idx_t division */

    /* Compute (n-2)! mod n */
    seq_t fact = 1 % mod;
    for (idx_t i = 2; i < n - 1; ++i)  /* i goes from 2 to n-2 inclusive */
    {
        fact = modmul(fact, (seq_t)i, mod);
    }

    /* Compute 2^(n-2) mod n */
    seq_t pow2 = modpow(2, n - 2, mod);

    /* Combine the parts */
    seq_t part = modmul(pow2, fact, mod);
    seq_t res = (part + floor_half) % mod;

    return res;
}
