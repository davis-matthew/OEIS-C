/*
 * A325113  Positive integers whose decimal representation has no nonzero
 *          subsequence that is divisible by 4.
 * https://oeis.org/A325113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 6627, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A325113 (positive integers whose
   decimal representation has no nonzero subsequence divisible by 4) by
   incremental search with caching. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*--- Utility: Check whether a given integer satisfies the conditions
   described in the OEIS entry. ------------------------------------*/
static bool is_valid(seq_t v)
{
    if (v == 0) return false;          /* only positive integers */

    /* Extract decimal digits (least‑significant first). */
    seq_t x = v;
    uint8_t dgt[200];
    size_t len = 0;
    while (x > 0) {
        dgt[len++] = (uint8_t)(x % 10);
        x /= 10;
    }

    bool all_prev_even = true;   /* all digits seen so far are even? */
    bool all_prev_odd  = true;   /* all digits seen so far are odd?  */
    bool zero_seen = false;

    for (long long i = (long long)len - 1; i >= 0; --i) {
        uint8_t d = dgt[(size_t)i];

        /* Forbidden digits 4 or 8. */
        if (d == 4 || d == 8) return false;

        /* If a 0 has already appeared, no later even digit is allowed. */
        if (zero_seen && (d == 0 || d == 2 || d == 6)) return false;

        if (d == 0) {
            /* All previous digits must be odd. */
            if (!all_prev_odd) return false;
            zero_seen = true;
            all_prev_even = false;   /* 0 is even */
        } else if (d == 2 || d == 6) {
            /* All previous digits must be even. */
            if (!all_prev_even) return false;
        }
        /* odd digits (1,3,5,7,9) are always acceptable. */

        /* Update parity flags for the current digit. */
        if ((d & 1) == 0) {          /* even */
            all_prev_odd = false;
        } else {                     /* odd */
            all_prev_even = false;
        }
    }
    return true;
}

/*--- Cache: stores previously found sequence terms. ----------------*/

/* In C++ we want to use new/delete; in C we use malloc/free. */
#ifndef CACHE_ALLOC
#if defined(__cplusplus)
#define CACHE_ALLOC new seq_t[ new_cap ]
#define CACHE_DELETE delete[] cache
#else
#define CACHE_ALLOC (seq_t*)malloc(new_cap * sizeof(seq_t))
#define CACHE_DELETE do { free(cache); } while(0)
#endif
#endif

static seq_t *cache = NULL;   /* dynamic array of sequence values */
static size_t cache_len = 0;  /* number of elements currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;

    /* Allocate new storage. */
    seq_t *temp = CACHE_ALLOC;
    if (!temp) exit(EXIT_FAILURE);

    /* Copy existing elements. */
    for (size_t i = 0; i < cache_len; ++i) temp[i] = cache[i];

    /* Free old storage. */
    CACHE_DELETE;

    cache = temp;
    cache_cap = new_cap;
}

/*--- Main function that returns the nth term. ---------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is the first term */

    /* Ensure cache contains at least n terms. */
    static seq_t last_candidate = 0;   /* last integer tested (for speed) */
    for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
        /* Find the next valid integer. */
        do { ++last_candidate; } while (!is_valid(last_candidate));

        grow_cache(cache_len + 1);
        cache[cache_len++] = last_candidate;
    }
    return cache[n - 1];
}
