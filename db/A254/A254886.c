/*
 * A254886  a(n) = least k>0 such that n-k^2 and n+k^2 are both primes.
 * https://oeis.org/A254886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A254886:
 *   a(n) = least k>0 such that n-k^2 and n+k^2 are both primes
 *   (returns 0 if no such k exists).
 * The function performs a simple search; no built‑in table is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* 64‑bit deterministic Miller–Rabin primality test */

static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static inline bool is_prime_u64(idx_t n)
{
    /* Handles values that fit in unsigned 64‑bit integers (idx_t is uint64_t). */
    uint64_t nn = (uint64_t)n;
    if (nn < 2)
        return false;
    if (nn == 2 || nn == 3)
        return true;
    if ((nn & 1ULL) == 0ULL)
        return false;

    /* Write nn‑1 as d * 2^s */
    uint64_t d = nn - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }

    /* Deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a >= nn)
            return true;                 /* all remaining bases are unnecessary */
        uint64_t x = mod_pow_uint64(a, d, nn);
        if (x == 1ULL || x == nn - 1)
            continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r)
        {
            x = mod_mul_uint64(x, x, nn);
            if (x == nn - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Sequence evaluation function */

seq_t a(idx_t n)
{
    if (n < 2)                 /* a(1)=0, plus any n<2 (for safety) */
        return (seq_t)0;

    /* Search for the smallest k>0 with both n-k^2 and n+k^2 prime. */
    for (idx_t k = 1; k * k < n; ++k)
    {
        idx_t low  = n - k * k;
        idx_t high = n + k * k;

        if (is_prime_u64(low) && is_prime_u64(high))
            return (seq_t)k;
    }
    return (seq_t)0;           /* no suitable k found */
}
