/*
 * A242552  Least number k such that n^4 + k^4 is prime.
 * https://oeis.org/A242552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n⁴ + k⁴ is prime. (search)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   is_prime_big, mul_mod64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/seq_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/mul_mod64.h"

/* 64‑bit modular multiplication (used by the Miller–Rabin test) */

/* 64‑bit modular exponentiation */
static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod64(result, b, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* OEIS sequence A242552: least k such that n^4 + k^4 is prime */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range; not used by OEIS */

    seq_t n4 = seq_pow((seq_t)n, 4);   /* n⁴ as a seq_t value */

    for (idx_t k = 1; ; ++k) {
        seq_t k4   = seq_pow((seq_t)k, 4);   /* k⁴ */
        seq_t sum  = n4 + k4;                /* n⁴ + k⁴ */

        /* use the generic big‑integer primality test */
        if (is_prime_big(sum))
            return (seq_t)k;

        /* fast path for values that fit in 64‑bit */
        if (sum <= (seq_t)UINT64_MAX) {
            if (is_prime64((uint64_t)sum))
                return (seq_t)k;
        }
    }
}
