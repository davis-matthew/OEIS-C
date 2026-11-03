/*
 * A366931  Least k such that 3*n*k+1 is a prime.
 * https://oeis.org/A366931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 3*n*k + 1 is prime. Computation:
 *            brute‑force search for k starting at 1, testing primality of
 *            3*n*k+1 with isPrimeGeneric
 * Range:     u8 n <= 27, u16 n <= 2900, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 102 terms through n = 4577 (2026-11-03)
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

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset is 1; behaviour below is irrelevant */

    seq_t sn   = (seq_t)n;        /* convert index to seq_t for multiplication */
    seq_t three = (seq_t)3;

    for (idx_t k = 1; ; ++k) {
        seq_t ks  = (seq_t)k;
        seq_t v   = three * sn * ks + (seq_t)1;
        if (isPrimeGeneric(v))
            return (seq_t)k;
    }
}
