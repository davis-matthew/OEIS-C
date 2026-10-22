/*
 * A307636  Numbers k with property that no two divisors of k share a common
 *          digit.
 * https://oeis.org/A307636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 3030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307636: numbers k such that no two divisors of k share a
   decimal digit.  Computed by an incremental search. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Convert a non‑zero integer to a 10‑bit mask of its decimal digits. */
static inline uint16_t digit_mask(uint64_t x)
{
    uint16_t m = 0;
    do {
        m |= 1u << (x % 10);
        x /= 10;
    } while (x);
    return m;
}

/* Return true if the divisors of k are pairwise digit‑disjoint. */
static bool divisors_disjoint(uint64_t k)
{
    uint16_t mask = 0;
    for (uint64_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            uint16_t m1 = digit_mask(d);
            if (m1 & mask) return false;
            mask |= m1;
            uint64_t e = k / d;
            if (e != d) {
                uint16_t m2 = digit_mask(e);
                if (m2 & mask) return false;
                mask |= m2;
            }
        }
    }
    return true;
}

/* Counts of terms found so far.  Cached values allow out‑of‑order calls. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t cur_k = 1;          /* next integer to test */

/* Ensure that cache has space for at least `need` entries. */
static void ensure_cache(size_t need)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < need) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1, graceful for n=0 */

    /* Find terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (divisors_disjoint(cur_k)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cur_k;
        }
        ++cur_k;
    }
    return cache[n - 1];
}
