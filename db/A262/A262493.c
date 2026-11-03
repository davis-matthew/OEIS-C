/*
 * A262493  Centered 13-gonal (or tridecagonal) primes.
 * https://oeis.org/A262493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form (13*k^2 - 13*k + 2)/2 for k ≥ 1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

/* Return the n‑th centered 13‑gonal prime (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)              /* the offset is 1; values for n < 1 are undefined */
        return 0;

    idx_t     k        = 1;           /* current k value */
    idx_t     found    = 0;           /* number of primes found so far */

    while (1) {
        /* compute (13*k^2 - 13*k + 2)/2 using seq_t arithmetic only */
        seq_t num = (seq_t)13 * (seq_t)k * (seq_t)k
                  - (seq_t)13 * (seq_t)k
                  + (seq_t)2;
        seq_t val = num / 2;

        if (isPrimeGeneric(val)) {
            ++found;
            if (found == n)
                return val;           /* found the desired term */
        }
        ++k;
    }
}
