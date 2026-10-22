/*
 * A308057  Lexicographically earliest sequence of positive terms such that
 *          for any distinct m and n, a(m) + 2*a(m+1) <> a(n) + 2*a(n+1).
 * https://oeis.org/A308057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the lexicographically earliest sequence defined by:
   a(1) = 1;
   for n ≥ 2, a(n) is the smallest positive integer such that
   f(n-1) = a(n-1) + 2·a(n) is distinct from all previous f-values.
   Here f(i) = a(i) + 2·a(i+1).  The implementation builds the sequence
   iteratively, memoises the found terms, and keeps a bitmap of used f-values.
   The construction reproduces all known terms of OEIS A308057. */
     
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static memoisation tables */
    static seq_t *seq_cache  = NULL;   /* a[1..]  (index 0 unused)        */
    static idx_t  seq_cap   = 0;       /* current capacity of seq_cache   */
    static idx_t  seq_len   = 0;       /* number of stored terms (seq_len-1 is last index) */

    static char *used_f     = NULL;    /* bitmap of f-values that have appeared   */
    static idx_t  used_cap  = 0;       /* current capacity of used_f              */

    /* Initialisation: allocate dummy zero element at index 0 */
    if (!seq_cache) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq_cache[0] = 0;      /* dummy, never accessed */
        seq_len = 1;           /* next free index will be 1 */
    }

    /* Initialise bitmap for used f-values */
    if (!used_f) {
        used_cap = 1024;                     /* starting size */
        used_f = (char *)calloc((size_t)used_cap, 1);
    }

    /* Grow the sequence until it contains term n */
    while (seq_len <= n) {

        /* Ensure the cache has room for the new element */
        if (seq_len >= seq_cap) {
            idx_t new_cap = seq_cap * 2;
            if (new_cap < 16) new_cap = 16;
            seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* First term a(1) = 1 */
        if (seq_len == 1) {
            seq_cache[seq_len] = (seq_t)1;
            seq_len++;
            continue;
        }

        seq_t prev = seq_cache[seq_len - 1];
        seq_t cand = (seq_t)1;

        /* Find the smallest candidate such that f = prev + 2*cand
           has not been used yet. */
        while (1) {
            seq_t f_val = prev + (seq_t)2 * cand;
            idx_t  f_idx = (idx_t)f_val;                /* assumes f_val fits into idx_t */

            /* Resize bitmap if necessary */
            if (f_idx >= used_cap) {
                idx_t new_cap = used_cap * 2;
                if (new_cap <= f_idx) new_cap = f_idx + 1;
                used_f = (char *)realloc(used_f, new_cap);
                memset(used_f + used_cap, 0, (size_t)(new_cap - used_cap));
                used_cap = new_cap;
            }

            if (!used_f[f_idx]) {
                seq_cache[seq_len] = cand;   /* store chosen term */
                used_f[f_idx] = 1;          /* mark f as used */
                break;
            }
            cand++;                          /* try next candidate */
        }

        seq_len++;   /* advance to next free index */
    }

    return seq_cache[n];
}
