/*
 * A266965  Primes of the form p = a^2 + b^2 where |a^2 - b^2| is composite.
 * https://oeis.org/A266965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266965: primes p = a² + b² with |a² – b²| composite.
   For each prime candidate p ≡ 1 (mod 4) we try every representation
   a² + b² = p (with a ≥ b).  If any such representation gives a
   composite difference a² – b², the prime is accepted.  The sequence
   is built by incremental search; values are cached so calls in any
   order are handled efficiently. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;      /* cache of found terms */
static size_t cache_len = 0;     /* number of elements in cache    */
static size_t cache_cap = 0;     /* allocated capacity              */
static uint64_t next_candidate = 5;   /* next odd integer to test for primality */

/* Ensure cache has room for one more element. */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *new_buf = seq_realloc(cache, cache_cap, new_cap);
        if (!new_buf) {
            /* Hard failure: terminate program */
            exit(EXIT_FAILURE);
        }
        cache = new_buf;
        cache_cap = new_cap;
    }
}

/* Generate terms until the cache contains at least n elements. */
static void generate_until(idx_t n)
{
    while (cache_len < n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* consider only odd numbers */

        if (!is_prime_big((seq_t)cand))
            continue;                 /* not a prime -> skip */

        /* Only primes ≡ 1 (mod 4) can be expressed as a²+b². */
        if ((cand & 3U) != 1U)
            continue;

        seq_t p   = (seq_t)cand;
        seq_t sqr = floor_sqrt(p);
        idx_t bound = (idx_t)sqr;

        bool ok = false;
        for (idx_t a_v = bound; a_v >= 1; --a_v) {
            seq_t a   = (seq_t)a_v;
            seq_t a2  = a * a;
            if (a2 > p)          /* safety; should not happen */
                continue;

            seq_t b2  = p - a2;
            seq_t b   = floor_sqrt(b2);
            if (b * b != b2)     /* not a perfect square */
                continue;

            /* Ensure a ≥ b to avoid negative differences. */
            if (a2 < b * b)
                continue;

            seq_t diff = a2 - b * b;   /* non‑negative */
            if (!is_prime_big(diff) && diff > 1) {
                ok = true;
                break;
            }
        }

        if (ok) {
            ensure_capacity();
            cache[cache_len++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but never called */
    generate_until(n);
    return cache[n - 1];
}
