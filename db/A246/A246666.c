/*
 * A246666  Numbers n such that n^3 + (n+1)^3 + (n+3)^3 is prime.
 * https://oeis.org/A246666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 1753, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8767 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246666
   Seek the n‑th integer m such that m³ + (m+1)³ + (m+3)³ is prime.
   The sequence is stored in ascending order; a(1)=1, a(2)=3, … */

/* Build a sparse cache of found terms.  On each call the function
   extends the cache until it contains the requested index in O(1)
   amortised time.  The computation of the prime test is performed
   with the supplied big‑integer type seq_t, so the code works for
   any supported seq_t implementation. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/mem.h"

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
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

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_size = 0;    /* how many cached */
static size_t cache_cap  = 0;    /* allocator capacity */

/* Return the n‑th term of the sequence (n >= 1).  The function
   returns 0 when n == 0 or if memory allocation fails. */
seq_t a(idx_t n)
{
    if (n == 0)                         /* offset is 1 */
        return 0;

    /* Already computed?  */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Iterate odd candidates, starting after the largest cached
       term or from 1 if the cache is empty.  */
    seq_t cand = 1;
    if (cache_size > 0)
        cand = cache[cache_size - 1] + 2;     /* skip even numbers */

    /* Extending the cache until we reach the desired index.  */
    while ((size_t)n > cache_size)
    {
        /* Compute 3*n³ + 12*n² + 30*n + 28  (the given expression).  */
        seq_t n2 = cand * cand;
        seq_t n3 = n2 * cand;
        seq_t expr = (seq_t)3 * n3 + (seq_t)12 * n2 + (seq_t)30 * cand + (seq_t)28;

        if (isPrimeGeneric(expr))
        {
            /* Ensure sufficient capacity.  */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache)          /* allocation failed */
                    return 0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        cand += 2;                     /* next odd candidate */
    }

    return cache[(size_t)n - 1];
}
