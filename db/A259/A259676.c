/*
 * A259676  Heptagonal numbers (A000566) that are semiprimes (A001358).
 * https://oeis.org/A259676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H(n) where H(n) = n*(5*n-3)/2 is a heptagonal number and
 *            a semiprime with exactly two (not necessarily distinct) prime
 *            factors
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 919 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Primality helpers provided by the utils library */
#include <stdint.h>
#include "../utils/bigomega_seq.h"
#include "../utils/is_prime_seq.h"
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

/* Count prime factors (with multiplicity) – a semiprime has exactly two. */

/* Helper to test if a value is a semiprime. */
static bool is_semiprime(seq_t x)
{
    /* 0,1,2,3 cannot be semiprime */
    if (x < 4) return false;
    return bigomega_seq(x) == 2;
}

/* Return the n-th element of A259676 (1‑based). */
seq_t a(idx_t n)
{
    /* Cache of previously found values for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_k = 0;   /* largest k examined so far */

    /* Grow the cache until it contains at least n values. */
    while (cache_size < n) {
        ++last_k;
        /* use a 64‑bit intermediate to avoid overflow for small seq_t's */
        uint64_t k = (uint64_t)last_k;
        uint64_t H64 = k * (5 * k - 3) / 2;
        seq_t H = (seq_t)H64;    /* cast to the actual seq_t type */

        if (is_semiprime(H)) {
            /* Ensure capacity. */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_size + 1)
                    new_cap = cache_size + 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* Allocation failed – abort; in contest harness this
                       will not happen, but we keep it for safety. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = H;
        }
    }

    return cache[n - 1];   /* a(1) is the first term stored. */
}
