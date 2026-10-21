/*
 * A250041  Numbers n such that m = floor(n/10) is not coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 4844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250041: numbers n such that
 *     m = floor(n/10) is NOT coprime to n, and if m ≠ 0 then m
 *     is also in the sequence.
 * Computation method: incremental search building a list of terms.
 * No hard‑coded table; memory grows as needed.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Persistent global state                                            */
/* ------------------------------------------------------------------ */
static bool *term_cache      = NULL;  /* term_cache[k] == true  ⇔  k is a term  */
static size_t cache_size     = 0;     /* allocated size (in bools)      */
static seq_t *term_list      = NULL;  /* list of terms in order         */
static size_t list_size      = 0;     /* number of terms found          */
static size_t list_cap       = 0;     /* allocated capacity of term_list*/
static idx_t  last_checked   = 1;     /* last integer examined          */

/* ------------------------------------------------------------------ */
/* Helper: grow term_cache as needed                                  */
/* ------------------------------------------------------------------ */
static void grow_cache(idx_t need)
{
    if ((size_t)need < cache_size)
        return;

    size_t new_size = cache_size ? cache_size : 1;
    while ((size_t)need >= new_size)
        new_size <<= 1;

    bool *tmp = (bool *)realloc(term_cache,
                                new_size * sizeof(bool));
    if (!tmp)
        return;                       /* silently abort on failure */

    /* zero the newly allocated part */
    for (size_t i = cache_size; i < new_size; ++i)
        tmp[i] = false;

    term_cache = tmp;
    cache_size = new_size;
}

/* ------------------------------------------------------------------ */
/* Helper: grow term_list as needed                                   */
/* ------------------------------------------------------------------ */
static void grow_list(void)
{
    if (list_size < list_cap)
        return;

    size_t new_cap = list_cap ? (list_cap << 1) : 64;
    seq_t *tmp = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
    if (!tmp)
        return;                       /* silently abort on failure */

    term_list = tmp;
    list_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --- initialization on first call -------------------------------- */
    if (term_cache == NULL) {
        cache_size = 16;
        term_cache = (bool *)calloc(cache_size, sizeof(bool));
        if (!term_cache)
            return 0;                 /* allocation failure → 0      */
        /* term_cache[0] and term_cache[1] remain false              */
    }

    /* --- generate terms until we have at least n of them ------------- */
    while (list_size < n) {
        ++last_checked;                 /* start at 2 on first iteration */
        grow_cache(last_checked);       /* ensure cache can store last_checked */

        bool is_term;
        if (last_checked < 10) {
            /* m = 0, gcd(last_checked,0) = last_checked > 1 for n >= 2 */
            is_term = (last_checked >= 2);
        } else {
            idx_t m = last_checked / 10;
            if (!term_cache[m]) {
                is_term = false;       /* m is not a term → reject */
            } else {
                seq_t x = (seq_t)last_checked;
                seq_t y = (seq_t)m;
                seq_t g = gcd(x, y);
                is_term = (g > (seq_t)1);
            }
        }

        term_cache[last_checked] = is_term;
        if (is_term) {
            grow_list();
            term_list[list_size++] = (seq_t)last_checked;
        }
    }

    /* --- return the requested term in order (a(1) == term_list[0]) --- */
    return term_list[n - 1];
}
