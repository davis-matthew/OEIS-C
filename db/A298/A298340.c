/*
 * A298340  a(n) = a(n-1) + a(n-2) + a([n/3]), where a(0) = 1, a(1) = 1, a(2)
 *          = 1.
 * https://oeis.org/A298340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a([n/3]), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache holding already computed terms  */
static seq_t *a_cache = NULL;      /* array a_cache[i] = a(i)   */
static size_t a_len   = 0;         /* number of elements stored (indices 0 .. a_len-1) */

/* Ensure that a_cache contains values up to index n inclusive */
static void ensure_upto(idx_t n)
{
    /* already computed? */
    if (a_len > 0 && n < a_len)
        return;

    /* index where we will start computing new terms */
    idx_t prev_len = (idx_t)a_len;

    /* Need more room for indices 0 .. n */
    size_t need = n + 1;
    if (need > a_len) {
        seq_t *tmp = (seq_t*)realloc(a_cache, need * sizeof(seq_t));
        if (!tmp) /* allocation failure – leave uncomputed */
            return;
        a_cache = tmp;
        a_len   = need;
    }

    /* Compute the new terms from prev_len up to n inclusive */
    for (idx_t i = prev_len; i <= n; ++i) {
        if (i == 0 || i == 1 || i == 2) {
            a_cache[i] = (seq_t)1;
        } else {
            a_cache[i] = a_cache[i - 1] + a_cache[i - 2] + a_cache[i / 3];
        }
    }
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return a_cache[(size_t)n];
}
