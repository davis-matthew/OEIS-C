/*
 * A299986  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 6, and no term occurs twice.
 * https://oeis.org/A299986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a lexicographically first permutation of nonnegative integers *
 *            such that a(n)+a(n+1) contains the decimal digit 6. * The
 *            algorithm: a(0)=0. For n>0, a(n) is the smallest unused *
 *            nonnegative integer x such that a(n-1)+x contains a digit 6. *
 *            This greedy choice yields the required lexicographic m...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   has_digit6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/has_digit6.h"

/* Cache for computed sequence values. */
static seq_t *seq_cache  = NULL;
static size_t cache_len  = 0;   /* number of terms currently cached */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Used flag array for numbers that have already appeared. */
static unsigned char *used   = NULL;   /* each index 0 (unused) or 1 (used) */
static size_t used_cap = 0;            /* allocated capacity for used[] */

/* Ensure used[] can hold at least 'size' entries. */
static void ensure_used_capacity(size_t size)
{
    while (used_cap < size) {
        size_t new_cap = used_cap * 2 + 128;
        unsigned char *new_ptr = (unsigned char *)realloc(used, new_cap);
        if (!new_ptr) abort();          /* allocation failure */
        /* Zero the newly allocated part. */
        memset(new_ptr + used_cap, 0, new_cap - used_cap);
        used = new_ptr;
        used_cap = new_cap;
    }
}

/* Ensure seq_cache can hold at least 'size' entries. */
static void ensure_cache_capacity(size_t size)
{
    while (cache_cap < size) {
        size_t new_cap = cache_cap * 2 + 16;
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        seq_cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Expand cache up to n. */
    while (cache_len <= (size_t)n) {
        if (cache_len == 0) {
            /* Initialize first term a(0)=0. */
            ensure_cache_capacity(1);
            seq_cache[0] = (seq_t)0u;
            cache_len = 1;
            /* Mark 0 as used. */
            ensure_used_capacity(1);
            used[0] = 1u;
            continue;
        }

        /* Compute a(cache_len) : next term. */
        seq_t prev = seq_cache[cache_len - 1];
        idx_t candidate = 0;

        /* Search for the smallest unused integer candidate such that
           prev + candidate contains a digit 6. */
        for (;;) {
            ensure_used_capacity((size_t)candidate + 1);
            if (!used[(size_t)candidate]) {
                seq_t sum = prev + (seq_t)candidate;
                if (has_digit6(sum)) {
                    /* Accept this candidate. */
                    used[(size_t)candidate] = 1u;
                    ensure_cache_capacity(cache_len + 1);
                    seq_cache[cache_len] = (seq_t)candidate;
                    ++cache_len;
                    break;
                }
            }
            ++candidate;
        }
    }

    return seq_cache[(size_t)n];
}
