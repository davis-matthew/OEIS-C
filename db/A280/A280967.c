/*
 * A280967  Vampire numbers (definition 1) in binary: n has a nontrivial
 *          factorization using n's bits.
 * https://oeis.org/A280967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A280967 – binary vampire numbers (definition 1).
   The function generates the sequence on demand, caching results. */

/* External types:  seq_t – unsigned integer or big‑int class,
   idx_t – unsigned integral type (uint64_t).  The build supplies them. */

#include <stdlib.h>
#include <stdbool.h>

/* ------- small helpers for 64‑bit arithmetic --------------------- */
/* Count bits (bit length).  For zero returns 0. */
static unsigned int
bitlen_uint64(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 0;
}

/* Popcount on 64‑bit. */
static unsigned int
popcnt_uint64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* ------- recursive search for a binary vampire factorisation ----- */
/* 'rem'   – remaining product to split
   'bits'  – remaining number of binary bits required
   'ones'  – remaining number of 1‑bits required
   'start' – smallest factor to try (to avoid permutations)
   'used'  – number of factors already chosen */
static bool
dfs_uint64(uint64_t rem, unsigned int bits, unsigned int ones,
           uint64_t start, unsigned int used)
{
    if (rem == 1) {
        return (bits == 0 && ones == 0 && used >= 2);
    }

    /* Try using the whole remainder as the final factor. */
    if (rem >= 2 && used >= 1) {
        unsigned int b = bitlen_uint64(rem);
        unsigned int o = popcnt_uint64(rem);
        if (b == bits && o == ones) {
            return true;
        }
    }

    uint64_t sq = (uint64_t) __builtin_sqrtl((long double)rem);
    for (uint64_t d = start; d <= sq; ++d) {
        if (rem % d == 0) {
            uint64_t q = rem / d;
            unsigned int b = bitlen_uint64(d);
            if (b > bits) continue;
            unsigned int o = popcnt_uint64(d);
            if (o > ones) continue;
            if (dfs_uint64(q, bits - b, ones - o, d, used + 1)) {
                return true;
            }
        }
    }
    return false;
}

static bool
is_binary_vampire_uint64(uint64_t v)
{
    unsigned int bits = bitlen_uint64(v);
    unsigned int ones = popcnt_uint64(v);
    return dfs_uint64(v, bits, ones, 2, 0);
}

/* ------- cache of found terms ---------------------------------- */
static seq_t *cache = NULL;   /* array holding the found terms */
static size_t cache_len = 0;  /* number of stored terms */
static size_t cache_cap = 0;  /* allocated capacity */

/* Append a term to the cache, growing if necessary. */
static void
cache_append(seq_t val)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------- main sequence function --------------------------------- */
seq_t
a(idx_t n)
{
    /* n == 0 is undefined for this sequence; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Ensure we have at least n terms cached. */
    if (cache_len < n) {
        /* Start searching from either the first known term or the
           next integer after the last cached value. */
        uint64_t cur = cache_len ? (uint64_t) cache[cache_len - 1] + 1
                                 : 159;   /* first term in the OEIS list */

        while (cache_len < n) {
            if (is_binary_vampire_uint64(cur))
                cache_append((seq_t) cur);
            ++cur;
        }
    }

    return cache[n - 1];
}
