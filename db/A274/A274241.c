/*
 * A274241  Numbers n such that there is a smaller positive number j == n (mod
 *          11) such that sqrt(j*n) is an integer.
 * https://oeis.org/A274241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 8460, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274241
   A(n) is the nth positive integer n such that there exists a smaller
   positive integer j ≡ n (mod 11) for which j · n is a perfect square. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Test whether a given integer n satisfies the definition.
   n is treated as uint64_t; all intermediate calculations fit in
   64‑bit unsigned arithmetic. */
static bool qualifies(uint64_t n)
{
    uint64_t temp = n;
    uint64_t a = 1;     /* square‑free part of n */
    uint64_t b = 1;     /* integer such that n = a·b² */

    /* --- factor out powers of 2 --- */
    unsigned int e2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e2;
    }
    if (e2) {
        if (e2 & 1U) a *= 2ULL;           /* odd part contributes to a */
        b <<= (e2 >> 1);                   /* even part contributes to b */
    }

    /* --- factor odd primes --- */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e & 1U) a *= p;          /* odd exponent contributes to a */
            /* e/2 copies of p go into b */
            uint64_t mul = 1;
            for (unsigned int i = 0; i < (e >> 1); ++i) mul *= p;
            b *= mul;
        }
    }

    if (temp > 1) {                     /* remaining prime factor has exponent 1 */
        a *= temp;
    }

    /* --- apply the characterization from the OEIS comment --- */
    if (a % 11ULL == 0ULL) {
        /* 11 divides the square‑free part: we only need a non‑trivial square factor */
        return b > 1ULL;
    } else {
        /* 11 does not divide the square‑free part.  Need d < b with d ≡ ±b (mod 11). */
        uint64_t r = b % 11ULL;
        uint64_t d = (r == 0ULL) ? 11ULL : r;
        if (d < b) return true;
        uint64_t r2 = (11ULL - r) % 11ULL;
        d = (r2 == 0ULL) ? 11ULL : r2;
        return d < b;
    }
}

/* ---- Caching state for the sequence ---- */
static seq_t   *seq      = NULL;   /* array holding found terms */
static size_t   seq_cap  = 0;      /* allocated capacity (elements) */
static size_t   seq_len  = 0;      /* current number of stored terms */
static uint64_t next_cand = 1;     /* next integer to test for qualification */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq,
                           new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --- Main function required by the build --- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined but unused for offset 1 */

    while (seq_len < n) {
        ++next_cand;
        if (qualifies((uint64_t)next_cand)) {
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)next_cand;
        }
    }
    return seq[n - 1];
}
