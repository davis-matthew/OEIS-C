/*
 * A247477  Largest m such that n/m + n divides (n/m)^(n/m) + n, (n/m)^n + n/m
 *          and n^(n/m) + n/m, or 0 if no such m exists.
 * https://oeis.org/A247477
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

/*  OEIS sequence A247477
 *
 *  a(n) = largest divisor m of n such that
 *
 *         let q = n / m
 *         k = q + n
 *         k | (q^q + n)      and
 *         k | (q^n + q)      and
 *         k | (n^q + q)
 *  If no divisor satisfies all three conditions, a(n) = 0.
 *
 *  The function computes the three checks by modular exponentiation
 *  using 64‑bit arithmetic with 128‑bit intermediate multiplication
 *  to avoid overflow.  Results are returned as seq_t; for any
 *  seq_t type that can hold the result the value is exact.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiply a*b modulo mod using unsigned __int128 for safety */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation: base^exp mod mod */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = mul_mod_uint64(result, b, mod);
        b = mul_mod_uint64(b, b, mod);
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    idx_t best = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        /* first candidate m = d */
        {
            idx_t m = d;
            idx_t q = n / m;
            uint64_t modv = (uint64_t)q + (uint64_t)n;

            uint64_t t1 = powmod_uint64((uint64_t)q, (uint64_t)q, modv);
            if ((t1 + (uint64_t)n) % modv != 0)
                goto next_candidate;

            uint64_t t2 = powmod_uint64((uint64_t)q, (uint64_t)n, modv);
            if ((t2 + (uint64_t)q) % modv != 0)
                goto next_candidate;

            uint64_t t3 = powmod_uint64((uint64_t)n, (uint64_t)q, modv);
            if ((t3 + (uint64_t)q) % modv != 0)
                goto next_candidate;

            if (m > best)
                best = m;
        }
next_candidate:
        /* second candidate m = n / d, if distinct */
        if (d != n / d) {
            idx_t m = n / d;
            idx_t q = n / m;
            uint64_t modv = (uint64_t)q + (uint64_t)n;

            uint64_t t1 = powmod_uint64((uint64_t)q, (uint64_t)q, modv);
            if ((t1 + (uint64_t)n) % modv != 0)
                continue;

            uint64_t t2 = powmod_uint64((uint64_t)q, (uint64_t)n, modv);
            if ((t2 + (uint64_t)q) % modv != 0)
                continue;

            uint64_t t3 = powmod_uint64((uint64_t)n, (uint64_t)q, modv);
            if ((t3 + (uint64_t)q) % modv != 0)
                continue;

            if (m > best)
                best = m;
        }
    }

    return (seq_t)best;
}
