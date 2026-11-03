/*
 * A347343  Positive integers k such that k with the last digit repeated is
 *          prime.
 * https://oeis.org/A347343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9349 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute A347343 by incremental search and cache.
   A(n) = the n-th positive integer k such that the number obtained by
   concatenating k with its last digit is prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* cached sequence values */
static seq_t *cache = NULL;     /* array of stored terms   */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* expand the cache to contain at least n terms */
static inline void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* determine starting candidate */
    idx_t cand = cache_len ? (idx_t)cache[cache_len - 1] + (idx_t)1 : (idx_t)1;

    /* maximum value that can be represented in seq_t (for fixed‑width types) */
    idx_t seqt_max = (idx_t)((seq_t)-1);
    bool seqt_fixed = ((seq_t)-1)    /* non‑zero for unsigned types */
                      && !((seq_t)-1 > seqt_max); /* avoid overflow for big ints */

    for (; cache_len < (size_t)n; ++cand) {
        /* stop if candidate would exceed the representable range of seq_t */
        if (seqt_fixed && cand > seqt_max) break;

        /* construct candidate * 10 + last digit */
        uint64_t last = cand % 10;
        seq_t concat = (seq_t)(cand * 10 + last);

        if (is_prime_big(concat)) {
            /* grow storage if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) { cache = NULL; return; }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offsets allowed but ignored */
    ensure_terms(n);
    if (cache_len < (size_t)n) return 0; /* no more terms (overflowed seq_t) */
    return cache[(size_t)n - 1];
}
