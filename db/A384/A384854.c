/*
 * A384854  The number of divisors d of n such that (-d)^d == d (mod n).
 * https://oeis.org/A384854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of proper divisors h of n such that (-h)^h = h
 *            (mod n)
 * Range:     u8 n <= 714, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>

/* Integer square root, floor of √n */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t x = (idx_t)sqrt((long double)n);
    while ((x + 1) <= n / (x + 1)) ++x;
    while (x > 0 && x > n / x) --x;
    return x;
}

/* Modular exponentiation: base^exp mod mod */
static inline seq_t pow_mod_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL) result = (result * base) % mod;
        exp >>= 1;
        if (exp) base = (base * base) % mod;
    }
    return result;
}

/* Test if divisor h of n satisfies (-h)^h ≡ h (mod n) */
static inline bool good_divisor(idx_t n, idx_t h)
{
    seq_t mod = (seq_t)n;
    seq_t h_seq = (seq_t)h;
    seq_t r = pow_mod_seq(h_seq, h, mod);
    if (h & 1ULL)          /* h odd: sign is negative */
    {
        seq_t comp = mod - r;          /* (-h)^h = -(h^h) ≡ mod - r (mod mod) */
        return comp == h_seq;
    }
    else                     /* h even: sign is positive */
    {
        return r == h_seq;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t sqrt_n = floor_isqrt_u64(n);
    seq_t count = 0;
    for (idx_t i = 1; i <= sqrt_n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i, d2 = n / i;
            if (d1 != n && good_divisor(n, d1)) ++count;
            if (d2 != d1 && d2 != n && good_divisor(n, d2)) ++count;
        }
    }
    return count + 1;   /* count proper divisors plus the divisor n itself */
}
