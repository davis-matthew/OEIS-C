/*
 * A299635  Solution (b(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A299635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299635 – the sequence b(n) from the system of complementary equations.
   a(0) = 1 ; b(0) = 2 ; c(0) = a(0)+b(0) = 3
   For n ≥ 1:
      a(n) = least new integer k ≥ 2*b(n-1)
      b(n) = least new integer k
      c(n) = a(n)+b(n)
   The function returns the value of b(n).  It lazily computes terms and
   stores them in a static cache, also maintaining a table of numbers that
   have already appeared in any a(n), b(n) or c(n) to enforce the “new” rule. */
/* NOTE: The implementation uses the helper functions  seq_alloc_new_array
   and  seq_free  (from the utils library) to handle storage of seq_t
   objects in a type‑safe manner that works for both built‑in integer
   types and arbitrary‑precision big integers. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static unsigned char *used = NULL;        /* boolean table of used values   */
static idx_t          used_size  = 0;    /* current capacity of 'used'      */

static seq_t *b_cache   = NULL;          /* cache of computed b(n) values   */
static idx_t  b_cache_len  = 0;           /* number of cached elements      */
static idx_t  b_cache_cap  = 0;           /* allocated capacity             */

static seq_t last_a = 0;                 /* a(n-1) of most recently computed term */
static seq_t last_b = 0;                 /* b(n-1) of most recently computed term */
static idx_t  next_unused = 0;            /* smallest integer not yet used   */

static bool initialized = false;          /* ensures one‑time setup          */

/*--- helpers -------------------------------------------------------------*/
static void ensure_used_size(idx_t needed)
{
    if ((idx_t)used_size > needed) return;
    size_t new_size = used_size ? used_size << 1 : 64;
    while (new_size <= (size_t)needed) new_size <<= 1;
    unsigned char *tmp = (unsigned char*)realloc(used, new_size);
    if (!tmp) exit(EXIT_FAILURE);
    /* zero the newly allocated part */
    memset(tmp + used_size, 0, new_size - used_size);
    used         = tmp;
    used_size    = (idx_t)new_size;
}

static void grow_b_cache(idx_t needed)
{
    if (needed < b_cache_cap) return;
    idx_t new_cap = b_cache_cap ? b_cache_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    for (idx_t i = 0; i < b_cache_len; ++i) new_arr[i] = b_cache[i];
    if (b_cache) seq_free(b_cache);
    b_cache = new_arr;
    b_cache_cap = new_cap;
}

static void init_state(void)
{
    if (initialized) return;
    initialized = true;

    /* initialise the used table with a small initial capacity */
    used_size = 64;
    used      = (unsigned char*)calloc(used_size, 1);
    if (!used) exit(EXIT_FAILURE);

    /* mark the initial values a0=1, b0=2, c0=3 as used */
    used[1] = used[2] = used[3] = 1;

    /* initialise the cache with b0 = 2 */
    b_cache_cap = 16;
    b_cache = seq_alloc_new_array(b_cache_cap);
    if (!b_cache) exit(EXIT_FAILURE);
    b_cache[0] = 2;
    b_cache_len  = 1;

    /* prepare for the first candidate search */
    last_a = 1;
    last_b = 2;

    /* find the first number that is not yet used */
    next_unused = 1;
    while (next_unused < (idx_t)used_size && used[next_unused]) ++next_unused;
}

/*--- public function ---------------------------------------------*/
seq_t a(idx_t n)
{
    init_state();

    if (n == 0) return b_cache[0];

    if (n >= b_cache_len) {
        grow_b_cache(n);
        for (idx_t i = b_cache_len; i <= n; ++i) {
            /* compute a(i) */
            seq_t cand_seq = last_b * (seq_t)2;
            idx_t  cand    = (idx_t)cand_seq;
            ensure_used_size(cand);
            while (used[cand]) { ++cand; ensure_used_size(cand); }
            last_a = (seq_t)cand;
            used[cand] = 1;

            /* compute b(i) */
            while (next_unused < used_size && used[next_unused]) ++next_unused;
            last_b = (seq_t)next_unused;
            used[next_unused] = 1;

            /* mark c(i) = a(i)+b(i) as used */
            seq_t c_val = last_a + last_b;
            idx_t  c_idx = (idx_t)c_val;
            ensure_used_size(c_idx);
            used[c_idx] = 1;

            /* store the new b(i) */
            b_cache[b_cache_len++] = last_b;
        }
    }

    return b_cache[n];
}
