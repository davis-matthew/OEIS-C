/*
 * A341890  Least k > 1 such that (n^n+k^k)/(n+k) is an integer.
 * https://oeis.org/A341890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341890: least k > 1 such that (n^n + k^k)/(n + k) is an integer */
#include <stdint.h>

/* fast modular exponentiation for 64‑bit arguments using __int128 for safety */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 3;                /* a(1) = 3 */
    uint64_t N = (uint64_t)n;            /* work in 64‑bit for speed */
    for (uint64_t k = 2; k <= N; ++k) {
        uint64_t m = N + k;              /* divisor (n + k) */
        uint64_t bn = mod_pow_uint64(N, N, m);   /* n^n mod m */
        uint64_t bk = mod_pow_uint64(k, k, m);   /* k^k mod m */
        if ((bn + bk) % m == 0)          /* integer condition */
            return (seq_t)k;
    }
    return (seq_t)N;                     /* fallback: a(n) ≤ n for n>1 */
}
