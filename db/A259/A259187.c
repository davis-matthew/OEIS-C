/*
 * A259187  Primes p such that both p and p^2 are distinct-digit numbers.
 * https://oeis.org/A259187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_counts, is_prime, seq_alloc, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that p and p² have all distinct decimal digits. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/digit_counts.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

/* Helper: tests whether all decimal digits of a number are distinct. */
static bool distinct_digits(seq_t x) {
    unsigned char cnt[10];
    digit_counts(x, cnt);
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] > 1) return false;
    }
    return true;
}

/* Helper: multiply a * b into res, returning true if the product overflows
   the type seq_t. Works for seq_t widths up to 8 bytes; otherwise assumes
   the multiplication does not overflow the test range. */
static bool mul_overflow(seq_t a, seq_t b, seq_t *res) {
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)a * (unsigned __int128)b;
        unsigned __int128 max = (unsigned __int128)((~ (seq_t)0));
        if (tmp > max) {
            *res = 0;
            return true;
        }
        *res = (seq_t)tmp;
        return false;
    } else {
        /* For 128‑bit or arbitrary–precision types the overflow is unlikely
           in the tested range; we simply multiply. */
        *res = a * b;
        return false;
    }
}

/* Internal cache of found primes. */
static seq_t *cache = NULL;
static idx_t cache_cnt = 0;
static idx_t cache_cap = 0;
static idx_t next_candidate = 2;

/* Public function required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, guard against 0 */

    /* Return cached value if already computed. */
    if (n <= cache_cnt) return cache[n - 1];

    /* Extend cache until it contains n terms. */
    while (cache_cnt < n) {
        /* Skip non‑primes. */
        while (!is_prime(next_candidate)) ++next_candidate;

        seq_t p = (seq_t)next_candidate;
        seq_t sq;
        if (!mul_overflow(p, p, &sq)) {
            if (distinct_digits(p) && distinct_digits(sq)) {
                /* Grow cache if needed. */
                if (cache_cnt == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *new_cache = seq_alloc(new_cap);
                    for (idx_t i = 0; i < cache_cnt; ++i) new_cache[i] = cache[i];
                    seq_free(cache);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_cnt++] = p;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
