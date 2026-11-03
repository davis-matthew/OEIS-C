/*
 * A294092  Numbers k == 119 (mod 120) such that 2^((k-1)/2), 3^((k-1)/2) and
 *          5^((k-1)/2) are congruent to 1 (mod k).
 * https://oeis.org/A294092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9369 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294092: Numbers k ≡ 119 (mod 120) for which
   2^((k‑1)/2), 3^((k‑1)/2) and 5^((k‑1)/2) ≡ 1 (mod k).
   Computation is performed by successive search of candidates
   of the form 120·m+119, testing the three modular
   exponentiations with the utility mod_pow.  Results are
   cached so that arbitrary order of calls is supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

seq_t a(idx_t n)
{
    /* Cache of found terms.  For C++ the cast of realloc is required. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* m is the counter used to generate candidates: k = 120*m + 119. */
    static idx_t m = 1;

    if (n == 0)
        return 0; /* undefined for n < 1 */

    /* Grow cache until we have at least n items. */
    while (cache_len < (size_t)n) {
        /* Ensure capacity for one more element. */
        grow_seq(&cache, &cache_cap, cache_len + 1);

        /* Candidate k. */
        seq_t k = (seq_t)120 * (seq_t)m + (seq_t)119;
        ++m;

        /* Exponent e = (k-1)/2; fit into 64‑bit for the required range. */
        uint64_t e = (uint64_t)((k - (seq_t)1) / (seq_t)2);

        /* Test the congruences. */
        if (powmod((seq_t)2, e, k) == (seq_t)1 &&
            powmod((seq_t)3, e, k) == (seq_t)1 &&
            powmod((seq_t)5, e, k) == (seq_t)1) {

            /* Store the new term in the cache. */
            cache[cache_len++] = k;
        }
    }

    return cache[n-1];
}
