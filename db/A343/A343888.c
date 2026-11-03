/*
 * A343888  Smallest positive integer such that the decimal representations of
 *          a(n) and of a(n)+9n (both without leading zeros) are permutations
 *          of each other.
 * https://oeis.org/A343888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 6235, u32 n <= 8999, u64 n <= 8999,
 *            u128 n <= 8999, big any n
 * Validated: standard, pass, 76 terms through n = 8860 (2026-11-03)
 * Helpers:   digit_frequencies, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest a(n) such that a(n) and a(n)+9n are digit permutations.
   The upper bound is 10 concatenated with n (i.e. 10·10^{digits(n)} + n). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Cache for memoization */
static seq_t *cache_vals   = NULL;
static bool  *cache_seen   = NULL;
static size_t cache_cap    = 0;

/* Ensure the cache can hold entries up to index n (0‑based). */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t  *new_vals = mem_new(seq_t, new_cap);
    bool   *new_seen = (bool  *)malloc(new_cap * sizeof(bool));
    if (!new_vals || !new_seen) return;    /* silently fail on OOM */

    if (cache_vals) {
        mem_copy(new_vals, cache_vals, cache_cap * sizeof(seq_t));
        memcpy(new_seen, cache_seen, cache_cap * sizeof(bool));
        mem_free(cache_vals);
        free(cache_seen);
    }

    mem_set(new_vals + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    memset(new_seen + cache_cap, 0, (new_cap - cache_cap) * sizeof(bool));

    cache_vals = new_vals;
    cache_seen = new_seen;
    cache_cap  = new_cap;
}

/* Compute a(n) by brute‑force search up to the claimed upper bound. */
static seq_t compute_a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t delta = n_seq * (seq_t)9;  /* 9·n */

    int d = num_digits_idx(n);                    /* number of decimal digits in n */
    seq_t upper = ((seq_t)10) * pow10((idx_t)d) + n_seq; /* 10·10^d + n */

    for (seq_t a = 1; a <= upper; ++a) {
        seq_t b = a + delta;
        unsigned int fa[10] = {0};
        unsigned int fb[10] = {0};

        digit_frequencies(a, fa);
        digit_frequencies(b, fb);

        bool equal = true;
        for (int i = 0; i < 10; ++i) {
            if (fa[i] != fb[i]) { equal = false; break; }
        }
        if (equal) return a;
    }
    return 0; /* should never happen */
}

/* Public function demanded by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    grow_cache(n);                        /* ensure cache capacity */

    if (cache_seen[(size_t)n]) return cache_vals[(size_t)n];

    seq_t result = compute_a(n);
    cache_vals[(size_t)n] = result;
    cache_seen[(size_t)n] = true;
    return result;
}
