/*
 * A355441  Numbers k such that the sum of the least prime factors of i=2..k
 *          is prime.
 * https://oeis.org/A355441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 340, u32 n <= 5676, u64 n <= 5676,
 *            u128 n <= 5676, big any n
 * Validated: standard, pass, 74 terms through n = 4993 (2026-11-03)
 * Helpers:   is_prime_u64, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355441: Numbers k such that the sum of the least prime factors of i=2..k is prime.
   The function builds the list incrementally, caching results so that the
   sequence can be asked in any order. */

/* The function uses the helpers _smallest_prime_factor(uint64_t) to obtain
   least prime factors and is_prime_big(seq_t) to test primality of the
   accumulated sum.  It dynamically grows a cache of the discovered
   k‑values. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/smallest_prime_factor.h"
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

static seq_t *term_cache = NULL;   /* array of discovered k-values      */
static size_t term_cap   = 0;      /* capacity of term_cache            */
static size_t term_len   = 0;      /* number of stored terms            */

static idx_t next_i       = 2;     /* next i to process                 */
static seq_t current_sum  = 0;     /* sum of lpf up to next_i-1         */

/* Grow the cache to at least new_cap elements. */
static void grow_terms(void)
{
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    seq_t *tmp = mem_resize(seq_t, term_cache, new_cap);
    if (!tmp) abort();                 /* out of memory – fatal              */
    term_cache = tmp;
    term_cap   = new_cap;
}

/* Expand the cache until it contains at least 'need' terms. */
static void ensure_terms(idx_t need)
{
    if (term_len >= need) return;

    /* Allocate initial cache if necessary. */
    if (!term_cache) grow_terms();

    for (;;) {
        uint64_t i = (uint64_t)next_i;
        uint64_t lpf = smallest_prime_factor(i);
        current_sum += (seq_t)lpf;

        if (is_prime_big(current_sum)) {
            if (term_len == term_cap) grow_terms();
            term_cache[term_len++] = (seq_t)next_i;
        }
        ++next_i;

        if (term_len >= need) break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1; guard case   */
    ensure_terms(n);
    return term_cache[n - 1];
}
