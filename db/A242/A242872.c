/*
 * A242872  Least number k > 1 such that (k^k-n^n)/(k-n) is an integer.
 * https://oeis.org/A242872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that (k^k - n^n)/(k-n) is an integer
 * Range:     u8 n <= 1814, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp > 0) {
        if (exp & 1)
            res = (uint64_t)((__int128)res * b % mod);
        b = (uint64_t)((__int128)b * b % mod);
        exp >>= 1;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* For n > 2, the answer is guaranteed to satisfy 2 ≤ k ≤ n-1 */
    for (idx_t k = 2; k <= n - 1; ++k) {
        uint64_t m = n - k;        /* k < n for n > 2, so m > 0 */
        seq_t p1 = mod_pow_uint64(k, k, m);
        seq_t p2 = mod_pow_uint64(n, n, m);
        if (p1 == p2)
            return (seq_t)k;
    }

    /* Should never be reached for valid inputs */
    return (seq_t)0;
}
