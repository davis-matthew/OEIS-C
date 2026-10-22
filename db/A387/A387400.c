/*
 * A387400  Number of nonnegative s < n such that s^s == s^n (mod n).
 * https://oeis.org/A387400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of s with 0 ≤ s < n satisfying s^s ≡ s^n (mod n)
 * Range:     u8 n <= 639, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* modular exponentiation for 64‑bit values.
   Uses 128‑bit arithmetic to avoid overflow. */
static inline uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;          /* any number ≡ 0 (mod 1) */
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Count the solutions to s^s ≡ s^n (mod n) for 0 ≤ s < n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;
    for (uint64_t s = 0; s < N; ++s) {
        uint64_t ss = powmod_u64(s, s, N);
        uint64_t sn = powmod_u64(s, N, N);
        if (ss == sn) ++cnt;
    }
    return (seq_t)cnt;
}
