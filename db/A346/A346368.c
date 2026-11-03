/*
 * A346368  Odd numbers that can be written in a single way as 2*k^2+p, k>0, p
 *          prime.
 * https://oeis.org/A346368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for odd numbers with a unique representation as 2*k^2 + prime */
/*    OEIS A346368: a(n) = the n-th smallest odd m such that
 *    m = 2*k^2 + p for exactly one pair (k>0, p prime).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cap    = 0;       /* capacity of cache */
    static size_t len    = 0;       /* number of stored terms */
    static seq_t last_od = 1;       /* last odd number tested */
    static idx_t  found   = 0;      /* how many terms have been found */

    /* initialise cache once */
    if (cache == NULL) {
        cap  = 4;            /* start small, grow as needed */
        len  = 0;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();
    }

    /* generate terms until we have at least n */
    while (found < n) {
        last_od += (seq_t)2;          /* next odd number */
        seq_t m = last_od;
        idx_t reps = 0;

        /* maximum k satisfies 2*k^2 <= m  ->  k <= sqrt(m/2) */
        seq_t half = m / (seq_t)2;
        idx_t max_k = (idx_t)floor_sqrt(half);

        for (idx_t k = 1; k <= max_k; ++k) {
            seq_t two_k2 = (seq_t)2 * (seq_t)k * (seq_t)k;
            seq_t p = m - two_k2;
            if (p <= 1) continue;
            if (is_prime_big(p)) {
                ++reps;
                if (reps > 1) break;   /* more than one representation */
            }
        }

        if (reps == 1) {                  /* unique representation */
            if (len == cap) {              /* grow cache if needed */
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = m;
            ++found;
        }
    }

    return cache[n - 1];
}
