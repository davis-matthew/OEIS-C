/*
 * A247200  Odd numbers which are neither of the form p*2^m + 1 nor of the
 *          form p*2^m - 1 with p prime.
 * https://oeis.org/A247200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9912 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for odd integers that are NOT of the
 * form p*2^m ± 1 where p is prime.  It returns the n‑th such number,
 * indexed with offset 1 (so a(1) = 71). 
 * The search uses the supplied helper isPrimeGeneric(seq_t) to test
 * primality of the odd component after removing all factors of 2.
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

/* Return true if x is of the forbidden form p*2^m ± 1 (p prime) */
static bool is_excluded(seq_t x)
{
    seq_t y, v;

    /* Test x - 1 */
    y = x - (seq_t)1;
    if (y != (seq_t)0) {
        /* p = 2 case: x-1 a power of two */
        if ((y & (y - (seq_t)1)) == (seq_t)0)
            return true;
        /* Remove all factors of 2 */
        v = y;
        while ((v & (seq_t)1) == (seq_t)0)
            v >>= 1;
        if (isPrimeGeneric(v))
            return true;
    }

    /* Test x + 1 */
    y = x + (seq_t)1;
    if (y != (seq_t)0) {
        if ((y & (y - (seq_t)1)) == (seq_t)0)
            return true;
        v = y;
        while ((v & (seq_t)1) == (seq_t)0)
            v >>= 1;
        if (isPrimeGeneric(v))
            return true;
    }

    return false;
}

/* Return the n‑th odd number that is not of the forbidden form */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset is 1 – not used */
        return (seq_t)0;

    seq_t cand = (seq_t)1;
    idx_t found = (idx_t)0;

    while (true) {
        if (!is_excluded(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        cand += (seq_t)2;   /* consider only odd numbers */
    }
}
