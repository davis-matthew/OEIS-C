/*
 * A384834  Number of divisors of n such that (-d)^d == -d (mod n).
 * https://oeis.org/A384834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the divisors d of n that satisfy (-d)^d ≡ -d (mod n). */
#include <stdbool.h>

static /* helper that works with 64‑bit integers */ unsigned __int128
mul_mod(unsigned __int128 a, unsigned __int128 b, unsigned long long mod)
{
    return (a * b) % mod;
}

/* modular exponentiation for 64‑bit integers – does not use the seq_t portion. */
static unsigned long long
mod_pow_idx(unsigned long long base, unsigned long long exp,
            unsigned long long mod)
{
    unsigned long long result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1UL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* below our offset – harmless to return 0 */
        return 0;
    seq_t count = 0;

    /* iterate over divisors up to sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* test divisor d */
            idx_t base1 = n - d;
            idx_t pow1 = mod_pow_idx(base1, d, n);
            if (pow1 == base1) ++count;

            /* test the complementary divisor n/d if distinct */
            idx_t other = n / d;
            if (other != d) {
                idx_t base2 = n - other;
                idx_t pow2 = mod_pow_idx(base2, other, n);
                if (pow2 == base2) ++count;
            }
        }
    }
    return count;
}
