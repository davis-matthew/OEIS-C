/*
 * A391471  Integers k such that floor((11/10)*k + 1/2) is a digit permutation
 *          of k.
 * https://oeis.org/A391471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 137, u32 n <= 484, u64 n <= 484,
 *            u128 n <= 484, big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391471: integers k for which
   T(k) = floor((11*k + 5)/10) is a digit permutation of k.
   This implementation generates the sequence by testing successive
   integers and caching the qualifying terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"

static seq_t *seq_cache = NULL;   /* cached terms */
static size_t seq_len  = 0;       /* number of cached terms */
static size_t seq_cap  = 0;       /* current allocation size */
static idx_t  next_k   = 1;       /* next integer to test */

/* Return true iff k satisfies the digit‑permutation condition. */
static bool is_term(seq_t k)
{
    seq_t t = (k * 11 + 5) / 10;                /* T(k) = floor((11*k+5)/10) */
    if (digit_len(k) != digit_len(t))
        return false;

    unsigned char cnt1[10] = {0};
    unsigned char cnt2[10] = {0};
    digit_counts(k, cnt1);
    digit_counts(t, cnt2);
    for (int d = 0; d < 10; ++d)
        if (cnt1[d] != cnt2[d])
            return false;
    return true;
}

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) /* out of memory – abort silently */
        return;
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* Return the nth term (1‑based).  Builds the cache lazily. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0 */

    /* Generate terms until we have at least n of them. */
    while ((idx_t)seq_len < n) {
        seq_t k = (seq_t)next_k;
        if (is_term(k)) {
            ensure_cache(seq_len + 1);
            seq_cache[seq_len++] = k;
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
