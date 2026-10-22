/*
 * A349492  a(1)=1, a(2)=6; for n > 2, a(n) is the smallest unused positive
 *          number such that gcd(a(n-1)+n, a(n)) > 1, gcd(a(n-1), a(n)) > 1,
 *          and gcd(n, a(n)) > 1.
 * https://oeis.org/A349492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(1)=1, a(2)=6; for n>2, a(n) is the smallest unused
 * positive integer x such that
 *   gcd(a(n‑1)+n , x)>1,
 *   gcd(a(n‑1)   , x)>1,
 *   gcd(n        , x)>1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *seq_cache = NULL;   /* array of computed a(n) */
static size_t seq_len   = 0;      /* number of computed terms */
static size_t seq_cap   = 0;      /* capacity of seq_cache */

static void grow_cache(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failed */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */
    while (seq_len < (size_t)n) {
        /* Compute next term */
        if (seq_len == 0) {            /* a(1) */
            grow_cache();
            seq_cache[seq_len++] = (seq_t)1;
            continue;
        }
        if (seq_len == 1) {            /* a(2) */
            grow_cache();
            seq_cache[seq_len++] = (seq_t)6;
            continue;
        }

        /* n > 2 */
        idx_t           idx      = (idx_t)(seq_len + 1); /* 1‑based index of new term */
        seq_t           a_prev   = seq_cache[seq_len - 1];
        seq_t           sum_prev = a_prev + (seq_t)idx;   /* a(n-1)+n */

        seq_t candidate = (seq_t)1;
        for (;;) {
            /* gcd conditions */
            if (gcd(sum_prev, candidate) == (seq_t)1) { ++candidate; continue; }
            if (gcd(a_prev, candidate)     == (seq_t)1) { ++candidate; continue; }
            if (gcd((seq_t)idx, candidate) == (seq_t)1) { ++candidate; continue; }

            /* check unused */
            bool used = false;
            for (size_t j = 0; j < seq_len; ++j) {
                if (seq_cache[j] == candidate) { used = true; break; }
            }
            if (used) { ++candidate; continue; }

            /* found the term */
            grow_cache();
            seq_cache[seq_len++] = candidate;
            break;
        }
    }
    return seq_cache[n - 1];
}
