/*
 * A359527  Nonnegative numbers k such that if 2^i and 2^j appear in the
 *          binary expansion of k, then 2^(i OR j) also appears in the binary
 *          expansion of k (where OR denotes the bitwise OR operator).
 * https://oeis.org/A359527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth nonnegative integer whose set of binary
 *            exponents is closed under bitwise OR (OEIS A359527). The
 *            implementation tests each integer for this closure property
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Test whether a number satisfies the closure under OR property. */
static bool union_closed_ok(seq_t x)
{
    if (x == 0)          /* 0 has no exponents – automatically closed */
        return true;

    /* collect the indices of the set bits */
    idx_t exp[128];
    idx_t cnt = 0;
    idx_t max_exp = 0;

    seq_t t = x;
    idx_t pos = 0;
    while (t) {
        if (t & 1) {
            exp[cnt++] = pos;
            if (pos > max_exp)
                max_exp = pos;
        }
        t >>= 1;
        ++pos;
    }

    /* check all unordered pairs */
    for (idx_t i = 0; i < cnt; ++i) {
        for (idx_t j = i + 1; j < cnt; ++j) {
            idx_t k = exp[i] | exp[j];
            if (k > max_exp) /* bit k cannot be set */
                return false;
            if (((x >> k) & 1) == 0)   /* bit k missing */
                return false;
        }
    }
    return true;
}

/* Cached sequence and state. */
static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cache_len = 0;    /* number of terms stored */
static idx_t  cache_cap = 0;    /* capacity of the array */
static seq_t  last_candidate = 0;  /* next integer to test */

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Grow the cache if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            grow_seq_array(&cache, &cache_cap, new_cap);
        }

        /* Test the current candidate. */
        if (union_closed_ok(last_candidate))
            cache[cache_len++] = last_candidate;

        /* Move to the next integer. */
        last_candidate++;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1: a(1)=0 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
