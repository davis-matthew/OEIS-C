/*
 * A338128  a(n) is the least k > 1 such that the base n representation of k^k
 *          ends with that of k.
 * https://oeis.org/A338128
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by searching for the smallest k>1 such that the
   base‑n representation of k^k ends with that of k.
   For k < n the condition reduces to k^k ≡ k (mod n).
   For k = n+1 the condition reduces to (n+1)^(n+1) ≡ n+1 (mod n^2). */

#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation for uint64_t arguments, safe for mod <= 1ULL<<63 */
static uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = (mod == 1) ? 0 : 1;
    base %= mod;
    while (exp) {
        if (exp & 1ull)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* External linkage as required.
   seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* ensure we work only for n >= 2 */
    if (n < 2)
        return (seq_t)0;          /* undefined for offset, return 0 */

    uint64_t N = (uint64_t)n;

    /* Search k = 2 .. n-1 */
    for (uint64_t k = 2; k < N; ++k) {
        uint64_t rem = powmod_u64(k, k, N);
        if (rem == k)
            return (seq_t)k;
    }

    /* Fallback: check k = n+1, which is the only candidate > n satisfying
       the bound a(n) <= n+1. */
    uint64_t k = N + 1;
    uint64_t mod = N * N;                 /* n^2 fits in 64 bits for N <= 10k */
    uint64_t rem = powmod_u64(k, k, mod);
    if (rem == k)
        return (seq_t)k;

    /* If no such k found (should not happen for n <= 10000) return n+1. */
    return (seq_t)k;
}
