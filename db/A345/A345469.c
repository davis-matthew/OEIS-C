/*
 * A345469  a(n) is the least k such that A345468(k) = 2*n-1.
 * https://oeis.org/A345469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that (2*n-1)^2 + 4*k is prime and
 *            (2*m-1)^2 + 4*k is composite for all 1 ≤ m < n
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    seq_t odd_n = (seq_t)(2 * n - 1);           /* 2*n-1 as seq_t */
    seq_t odd_n_sq = odd_n * odd_n;            /* (2*n-1)^2 */

    for (idx_t k = 1; ; ++k) {
        seq_t val_n = odd_n_sq + (seq_t)4 * k;
        if (!isPrimeGeneric(val_n))
            continue;

        bool ok = true;
        for (idx_t m = 1; m < n; ++m) {
            seq_t odd_m = (seq_t)(2 * m - 1);
            seq_t val_m = odd_m * odd_m + (seq_t)4 * k;
            if (isPrimeGeneric(val_m)) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)k;
    }
}
