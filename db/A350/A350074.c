/*
 * A350074  Difference between the maximal digit in the primorial base
 *          expansion of n and the maximal exponent in the prime factorization
 *          of n.
 * https://oeis.org/A350074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A051903(A276086(n)) - A051903(n)
 * Range:     i8 n <= 9902, i16 n <= 9902, i32 n <= 9902, i64 n <= 9902,
 *            i128 n <= 9902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*  Computes the OEIS sequence A350074:  a(n) = max digit in the primorial
    base representation of n  minus  the maximal exponent in the prime
    factorisation of n.                                 */

seq_t a(idx_t n)
{
    /* --- 1.  Pre‑compute primes and primorials ------------------------------- */
    static uint32_t primes[200];
    static uint64_t primorials[200];
    static size_t primes_len  = 0;   /* number of stored primes           */
    static size_t prim_len    = 0;   /* length of primorials array        */

    if (primes_len == 0) {                /* initialisation */
        primes[0]      = 2;
        primes_len     = 1;
        primorials[0]  = 1;                /* 0# = 1  */
        primorials[1]  = 2;                /* 1# = 2  */
        prim_len      = 2;
    }

    /* Extend the lists while the last primorial does not exceed n. */
    while (primorials[prim_len - 1] <= n) {
        /* find next prime after the last stored one */
        uint32_t candidate = primes[primes_len - 1] + 1;
        while (true) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint32_t p = primes[i];
                if ((uint64_t)p * p > candidate) break;
                if (candidate % p == 0) { is_prime = false; break; }
            }
            if (is_prime) break;
            ++candidate;
        }

        /* append the new prime and its primorial */
        primes[primes_len++] = candidate;
        primorials[prim_len] = primorials[prim_len - 1] * (uint64_t)candidate;
        ++prim_len;

        /* safety: stop if arrays are full (never happens for the intended limits) */
        if (prim_len >= sizeof(primorials) / sizeof(primorials[0]))
            break;
    }

    /* --- 2.  Maximal digit in the primorial base expansion  ---------------- */
    uint64_t remaining = n;
    uint64_t max_digit = 0;
    int i = (int)prim_len - 1;
    for (; i >= 0; --i) {
        uint64_t p = primorials[i];
        if (remaining < p) continue;
        uint64_t d = remaining / p;
        if (d > max_digit) max_digit = d;
        remaining -= d * p;
        if (remaining == 0) break;
    }

    /* --- 3.  Maximal exponent in the prime factorisation ------------------- */
    uint64_t temp       = n;
    uint64_t max_exp    = 0;

    /* factor 2 */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) {
        ++e;
        temp >>= 1;
    }
    if (e > 0 && e > max_exp) max_exp = e;

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > max_exp) max_exp = e;
        }
    }
    if (temp > 1 && max_exp == 0) max_exp = 1;   /* remaining prime factor */

    /* --- 4.  Result -------------------------------------------------------- */
    seq_t result = (seq_t)max_digit - (seq_t)max_exp;
    return result;
}
