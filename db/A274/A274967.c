/*
 * A274967  Odd composite numbers n which are not m-gonal number for 3 <= m <
 *          n.
 * https://oeis.org/A274967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 710, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9816 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274967: odd composite numbers that are not m‑gonal for 3 ≤ m < n.
   Computed by brute‑force scanning.  Uses the supplied utilities for primality
   testing and integer square root.  The function a(idx_t n) returns the nth
   term (n ≥ 1). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return true if n is an odd composite number (n ≥ 7). */
static bool is_composite_odd(seq_t n)
{
    if (n < 7)                    return false;   /* too small */
    if ((n & 1) == 0)             return false;   /* even numbers are excluded */
    if (isPrimeGeneric(n))        return false;   /* primes are excluded */
    return true;
}

/* Return true if n is an m‑gonal number for some 3 ≤ m < n. */
static bool is_m_gonal(seq_t n)
{
    /* 2n = k * (m*(k‑1) – 2k + 4).  For a given k, solve for m:
     * m = (2n/k + 2k – 4) / (k‑1).  Only k that divides 2n are viable. */
    seq_t two_n = n + n;
    seq_t sqrt2n = floor_sqrt(two_n);           /* kmax ≈ √(2n) */
    idx_t kmax = (idx_t)sqrt2n;                 /* safe: usually far below 2^64 */

    for (idx_t k = 3; k <= kmax; ++k) {
        seq_t kseq = (seq_t)k;
        if ((two_n % kseq) != 0) continue;     /* k must divide 2n */

        seq_t div = two_n / kseq;              /* 2n/k */
        seq_t m_num = div + 2 * kseq - (seq_t)4;/* numerator of m */
        seq_t denom = kseq - (seq_t)1;
        if ((m_num % denom) != 0) continue;    /* m must be integer */

        seq_t m = m_num / denom;
        if (m >= (seq_t)3 && m < n) return true; /* non‑trivial representation */
    }
    return false;
}

/* Static cache of computed sequence values. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_odd = 7;          /* next odd candidate to examine */

/* Grow the cache so that it can hold at least 'needed' terms. */
static void ensure_cache(size_t needed)
{
    if (cache_len >= needed) return;

    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    while (cache_len < needed) {
        seq_t cand = next_odd;
        next_odd += 2;                    /* next odd number */

        if (!is_composite_odd(cand)) continue;
        if (is_m_gonal(cand)) continue;

        cache[cache_len++] = cand;
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined; return 0 */
    ensure_cache((size_t)n);
    return cache[(size_t)(n - 1)];
}
