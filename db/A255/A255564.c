/*
 * A255564  Primes having in binary representation a nonprime number of 1's.
 * https://oeis.org/A255564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 4208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9825 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Sequence A255564: primes whose binary representation has a non‑prime
 *  number of 1‑bits (Hamming weight).  
 *  The function returns the n‑th such prime (n ≥ 1), using lazy caching
 *  so that calls in any order are efficient.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*  is_prime_small[i] is true iff i is a prime, 0 ≤ i ≤ 64.  */
static const bool is_prime_small[65] = {
    /*  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 */
    0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0,
    /* 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 */
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    /* 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 */
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0,
    /* 63 64 */
    0, 0
};

/*  Cached sequence values.  */
static seq_t *cache    = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/*  Candidate counter (starts at 0, increments until the first prime found). */
static seq_t next_candidate;
static bool next_candidate_set = false;

/*  Count 1‑bits of an arbitrary‑precision integer x.  */
static unsigned popcount_seq_t(seq_t x) {
    unsigned c = 0;
    while (x != (seq_t)0) {
        if (x & (seq_t)1) ++c;
        x >>= 1;
    }
    return c;
}

/*  Test whether a candidate satisfies the definition.  */
static bool candidate_is_included(seq_t cand) {
    if (!is_prime_big(cand)) return false;   /* candidate must be prime */

    unsigned pc = popcount_seq_t(cand);
    if (pc > 64) {          /* unlikely for the used range, treat as composite */
        /* simple trial division for pc > 64 */
        for (unsigned d = 2; d * d <= pc; ++d)
            if (pc % d == 0) return true;   /* non‑prime Hamming weight */
        return false;          /* Hamming weight itself is prime */
    }
    return !is_prime_small[pc];
}

/*  Ensure the cache contains at least n terms.  */
static void ensure_cache(idx_t n) {
    if (!next_candidate_set) {
        next_candidate = (seq_t)0;
        next_candidate_set = true;
    }
    while (cache_len < n) {
        if (candidate_is_included(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/*  Public function required by the problem statement.  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined for offset 0, harmless */
    ensure_cache((idx_t)n);
    return cache[n - 1];
}
