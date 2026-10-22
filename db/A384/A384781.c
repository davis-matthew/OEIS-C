/*
 * A384781  Number of values of s, 0 < s <= n - 1, such that (-s)^s == s (mod
 *          n).
 * https://oeis.org/A384781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of s, 0 < s ≤ n‑1, such that (-s)^s ≡ s (mod n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_mul.h"

/* modular exponentiation with a 64‑bit modulus (idx_t) */
static inline idx_t mod_pow_idx(idx_t base, idx_t exp, idx_t mod)
{
    idx_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1ULL) result = mod_mul(result, base, mod);
        exp >>= 1ULL;
        if (exp)      base   = mod_mul(base,   base,   mod);
    }
    return result;
}

/* a(idx_t n) – sequence A384781 */
seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n <= 1) return 0;           /* no s in the range */

    for (idx_t s = 1; s < n; ++s)
    {
        /* (-s)^s mod n == s :  (-s) mod n is n‑s for 0 < s < n   */
        idx_t pw = mod_pow_idx(n - s, s, n);
        if (pw == s) ++count;
    }
    return count;
}
