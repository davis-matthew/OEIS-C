/*
 * A274436  Numbers having fewer distinct prime factors of form 3*k+1 than of
 *          the form 3*k+2.
 * https://oeis.org/A274436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9983 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A274436: the n‑th positive integer whose distinct prime divisors of
   the form 3k+1 are fewer than those of the form 3k+2. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static bool initialized = false;
static idx_t spf_limit = 0;
static idx_t *spf = NULL;
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t next_candidate = 2;

static void ensure_initialized(void) {
    if (!initialized) {
        /* A safe upper limit for n up to ~10⁴ (customize if needed) */
        spf_limit = 5000000;
        build_spf(spf_limit, &spf);
        cache = NULL;
        cache_cap = 0;
        cache_size = 0;
        next_candidate = 2;
        initialized = true;
    }
}

static void ensure_cache_size(idx_t need) {
    ensure_initialized();
    while (cache_size < need) {
        idx_t m = next_candidate;
        idx_t c1 = 0, c2 = 0;
        while (m > 1) {
            idx_t p = spf[m];
            m /= p;
            while (m % p == 0) m /= p;
            if (p % 3 == 1) ++c1;
            else if (p % 3 == 2) ++c2;
        }
        if (c1 < c2) {
            if (cache_size + 1 > cache_cap) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
            }
            cache[cache_size] = (seq_t)next_candidate;
            ++cache_size;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cache_size(n);
    return cache[n - 1];
}
