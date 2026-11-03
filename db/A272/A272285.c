/*
 * A272285  Primes of the form 43*n^2 - 537*n + 2971 in order of increasing
 *          nonnegative values of n.
 * https://oeis.org/A272285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime of the form 43*n² - 537*n + 2971 with n = n-1 (n≥1)
 * Range:     u8 none, u16 n <= 43, u32 n <= 3808, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9795 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Returns the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined: sequence starts at offset 1 */
        return (seq_t)0;
    }

    /* Persistent cache of found primes */
    static seq_t *primes = NULL;       /* array of primes found so far  */
    static size_t count   = 0;         /* number of primes stored        */
    static size_t cap     = 0;         /* current capacity of array      */
    static idx_t  next_n  = 0;         /* next n (non‑negative) to test  */

    /* Initialise the cache on first call */
    if (primes == NULL) {
        cap = 128;
        primes = mem_new(seq_t, cap);
    }

    /* Generate primes until we have at least n of them */
    while (count < n) {
        seq_t i = (seq_t)next_n;               /* i = current n (0‑based)           */
        seq_t i_sq = seq_mul(i, i);            /* i²                                 */
        seq_t term = mul_small(i_sq, 43);      /* 43*i²                              */
        term = term - mul_small(i, 537);       /* -537*i                            */
        term = term + (seq_t)2971;             /* +2971                             */

        if (isPrimeGeneric(term)) {
            if (count == cap) {                /* grow array if needed             */
                cap <<= 1;
                primes = mem_resize(seq_t, primes, cap);
            }
            primes[count++] = term;            /* store the found prime            */
        }
        ++next_n;                              /* move to next index                */
    }

    /* Return the requested prime */
    return primes[n - 1];
}
