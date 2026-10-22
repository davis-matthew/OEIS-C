/*
 * A304451  Solution (a(n)) of the complementary equation in Comments.
 * https://oeis.org/A304451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304451: 
   recursive definition
   b(0) = 1
   b(n) = least positive integer not yet used (in the set of all
   previously computed b(k) and a(k))
   a(n) = b(n) + b(floor(n/2))
   offset = 0
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *b_cache = NULL;          /* stores b(k) values          */
static seq_t *a_cache = NULL;          /* stores a(k) values          */
static size_t cache_cap = 0;           /* capacity of the cache arrays */

static bool *used = NULL;              /* bitmap of used integers     */
static size_t used_cap = 0;            /* capacity of the bitmap      */

/* highest index computed so far; (idx_t)-1 means none */
static idx_t computed_n = (idx_t)-1;

/* smallest unused positive integer so far */
static idx_t next_candidate = 1;

/* Ensure the cache arrays have at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp_b = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    if (tmp_b) b_cache = tmp_b;

    seq_t *tmp_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (tmp_a) a_cache = tmp_a;

    cache_cap = new_cap;
}

/* Ensure the used bitmap can index up to 'needed'. */
static void ensure_used_cap(size_t needed)
{
    if (needed < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;

    bool *tmp_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp_used)
        return;                       /* allocation failure → keep old bitmap */

    /* initialise new entries to false */
    if (new_cap > used_cap)
        memset(tmp_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));

    used = tmp_used;
    used_cap = new_cap;
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    /* make sure we have room for indexes up to n */
    ensure_cache_cap((size_t)n + 1);

    for (idx_t k = computed_n + 1; k <= n; ++k)
    {
        /* --- find the least unused integer for b(k) --- */
        while (true) {
            ensure_used_cap((size_t)next_candidate);
            if (!used[(size_t)next_candidate])
                break;
            ++next_candidate;
        }
        seq_t b_k = (seq_t)next_candidate;
        ensure_used_cap((size_t)b_k);
        used[(size_t)b_k] = true;          /* mark b_k as used */

        /* --- compute a(k) = b(k) + b(floor(k/2)) --- */
        seq_t a_k;
        if (k == 0)
            a_k = b_k + b_k;                 /* b(0) + b(0) */
        else {
            idx_t half = k >> 1;              /* floor(k/2) */
            a_k = b_k + b_cache[(size_t)half];
        }
        ensure_used_cap((size_t)a_k);
        used[(size_t)a_k] = true;          /* mark a_k as used */

        /* store results */
        b_cache[(size_t)k] = b_k;
        a_cache[(size_t)k] = a_k;

        ++next_candidate;                  /* advance for next search */
    }

    computed_n = n;
    return a_cache[(size_t)n];
}
