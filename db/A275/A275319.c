/*
 * A275319  Numbers n such that n concatenated with n+1 is not a prime.
 * https://oeis.org/A275319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1006, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9928 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) = n such that the concatenation of n and n+1 is composite.
   This implements OEIS sequence A275319. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/*  Utility functions provided by the build environment.  */

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
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

/*  Cache for computed terms.  */
static seq_t *cache = NULL;      /* array of sequence values */
static size_t cache_size = 0;    /* number of elements stored */
static size_t cache_cap   = 0;    /* allocated capacity */
static idx_t next_candidate = 1; /* next integer to test for inclusion */

/*  Ensure cache has at least `needed` capacity.  */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();                /* allocation failure -> terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/*  Generate terms until we have at least `n` elements stored.  */
static void fill_cache_to(idx_t n) {
    if (n <= cache_size) return;
    /*  We may need to test many candidates; continue from last state.  */
    while (cache_size < n) {
        /*  Compute concatenation of candidate and candidate+1.  */
        seq_t concatenated = concat_parts(next_candidate, next_candidate + 1);
        /*  If concatenated is not prime, store the candidate.  */
        if (!is_prime_big(concatenated)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/*  OEIS sequence A275319: numbers n such that concat(n, n+1) is composite.  */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 0, but return 0 */
    fill_cache_to(n);
    return cache[n - 1];
}
