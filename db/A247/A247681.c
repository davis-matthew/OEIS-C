/*
 * A247681  Odd nonprimes congruent to 1 modulo 9.
 * https://oeis.org/A247681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th odd composite number congruent to 1 (mod 9);
 *            a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 2562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9797 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utils provide a generic prime test. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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
#include <stdbool.h>
#include <stdlib.h>

/* Memoisation table for already computed terms. */
static seq_t *cache = NULL;      /* array of previous terms          */
static size_t  cap   = 0;        /* current allocated capacity       */
static size_t  len   = 0;        /* number of terms stored           */
static idx_t   next_m = 0;       /* next multiplier m to try         */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_capacity(size_t need) {
    if (cap < need) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < need) newcap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) abort(); /* abort on memory exhaustion */
        cache = tmp;
        cap   = newcap;
    }
}

/* Return the n‑th odd nonprime ≡ 1 (mod 9).  n >= 1. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;        /* no a(0) defined */

    /* Generate further terms until we have at least n of them. */
    while (len < (size_t)n) {
        /* Numbers of the form 9·m + 1.  m starts at 0 and grows. */
        seq_t candidate = (seq_t)9 * (seq_t)next_m + (seq_t)1;
        next_m++;                              /* next m for next round */

        /* Skip even candidates: we need only odd numbers. */
        if ((candidate & (seq_t)1) == 0) continue;

        /* Candidate is nonprime if it is 1 or the generic prime test
           says it is not prime. */
        bool nonprime = (candidate == (seq_t)1) || !isPrimeGeneric(candidate);
        if (nonprime) {
            ensure_capacity(len + 1);
            cache[len++] = candidate;
        }
    }
    return cache[n - 1];
}
