/*
 * A303379  Primes p such that p == 1 (mod 6), p-2 is composite, and for all r
 *          in the range 5 <= r < p - 2, p-2 is not congruent to r (mod r*(r +
 *          1)/2).
 * https://oeis.org/A303379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303379:
 * Find primes p ≡ 1 (mod 6) such that (p-2) is composite and
 * for every integer r with 5 ≤ r < p-2,   (p-2) ≠ r (mod T(r))
 * where T(r) = r(r+1)/2 is the r‑th triangular number.
 * The function builds the sequence lazily and memoises results
 * to handle out‑of‑order calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers ---------------------------------------------------------- */
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

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

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state for memoisation ---------------------------------------- */
    static idx_t  found = 0;      /* number of terms already found      */
    static idx_t  last = 7;       /* next odd candidate (≡ 1 mod 6)     */
    static seq_t *cache = NULL;   /* array of found primes              */
    static size_t cache_cap = 0;  /* capacity of the cache              */

    /* Allocate or grow cache as needed ------------------------------------- */
    if (n > found) {
        size_t need = n;
        if (cache_cap < need) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort();            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search consecutive candidates until the n‑th term is found ----- */
        for (;;) {
            if (found >= n) break; /* safety guard */

            seq_t p = (seq_t)last;
            if (!is_prime_big(p)) {
                last += 6;
                continue;
            }

            seq_t m = p - (seq_t)2;
            if (is_prime_big(m)) { /* must be composite */
                last += 6;
                continue;
            }

            /* Compute rmax = floor((√(8m+1)-1)/2) */
            seq_t val   = (m << 3) + (seq_t)1;   /* 8*m + 1 */
            seq_t sqrtv = floor_sqrt(val);
            seq_t rmax  = (sqrtv - (seq_t)1) >> 1; /* divide by 2 */

            /* Convert rmax to idx_t for loop bounds (safe for n<10000) */
            idx_t rmax_i = (idx_t)rmax;

            /* Check the congruence condition for all r in [5, rmax] */
            bool ok = true;
            for (idx_t r = 5; r <= rmax_i; ++r) {
                seq_t tri = ((seq_t)r * (r + (seq_t)1)) / (seq_t)2;
                if ((m % tri) == (seq_t)r) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                cache[found++] = p;            /* store prime */
            }
            last += 6; /* move to next candidate ≡ 1 (mod 6) */
        }
    }

    return cache[n - 1];
}
