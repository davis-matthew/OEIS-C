/*
 * A291416  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 4 S + S^2.
 * https://oeis.org/A291416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A291416
    Recurrence: a(n) = 4*a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4)   for n >= 5
    Initial values: a(0)=4, a(1)=19, a(2)=86, a(3)=392, a(4)=1784
*/

#include <stdlib.h>

/* ------- static cache ---------------------------------------------------- */

static seq_t *cache = NULL;     /* pointer to the array of terms           */
static idx_t  cache_len = 0;    /* number of terms currently computed     */
static idx_t  cache_cap = 0;    /* allocated capacity (in terms)           */

/* Forward declaration of a helper used during expansion */
static seq_t get_term(idx_t idx);

/* Complete the cache up to index n (inclusive) and compute any missing terms */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len) return;               /* already available */

    idx_t new_cap = n + 1;                        /* we need at least n+1 terms */
    if (cache_cap < new_cap) {
        /* grow the array to the required size */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – terminate */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute the newly requested terms */
    for (idx_t i = cache_len; i < new_cap; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)4;
        } else if (i == 1) {
            val = (seq_t)19;
        } else if (i == 2) {
            val = (seq_t)86;
        } else if (i == 3) {
            val = (seq_t)392;
        } else if (i == 4) {
            val = (seq_t)1784;
        } else {
            /* recurrence: a(i)=4*a(i-1)+3*a(i-2)-2*a(i-3)-a(i-4) */
            val = (seq_t)4 * get_term(i-1);
            val += (seq_t)3 * get_term(i-2);
            val -= (seq_t)2 * get_term(i-3);
            val -= get_term(i-4);
        }
        cache[i] = val;
    }
    cache_len = new_cap;
}

/* Return the stored term (no bounds checking – caller is required to be correct) */
static seq_t get_term(idx_t idx)
{
    return cache[idx];
}

/* ------- public sequence function ------------------------------------- */

/* External linkage – required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
