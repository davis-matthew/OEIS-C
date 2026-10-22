/*
 * A329984  a(1) = 0 and for n > 0, a(n+1) is the odd part of k where k is the
 *          number of terms equal to a(n) among the first n terms.
 * https://oeis.org/A329984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 0; for n>0, a(n+1) = odd_part( count of a(n) among first
 *            n terms )
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/odd_part.h"

/* Compute the odd part of a positive integer (remove all factors of 2). */

/* Grow a seq_t array to hold at least min_cap elements.
   New elements are zeroed. */
static void ensure_seq_capacity(idx_t *cap, seq_t **arr, idx_t min_cap)
{
    if (*cap >= min_cap)
        return;
    idx_t old_cap = *cap;
    idx_t new_cap = (old_cap == 0) ? 1 : old_cap;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();
    *arr = tmp;
    for (idx_t i = old_cap; i < new_cap; ++i)
        (*arr)[i] = 0;
    *cap = new_cap;
}

/* Static sequence state shared across calls. */
static seq_t *seq_arr      = NULL; /* sequence terms (1‑indexed) */
static idx_t  seq_cap      = 0;     /* current capacity of seq_arr */
static idx_t  seq_max_n    = 0;     /* largest n computed so far */

static seq_t *count_arr    = NULL; /* occurrence counters, indexed by value */
static idx_t  count_cap    = 0;     /* current capacity of count_arr */

/* Return a(n) for n ≥ 1.  The function is memoised and works for
   arbitrary call order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 – return 0 */

    /* initialisation on first call */
    if (seq_max_n == 0) {
        ensure_seq_capacity(&seq_cap, &seq_arr, 2);   /* index 1 needed */
        ensure_seq_capacity(&count_cap, &count_arr, 2); /* indexes 0 and 1 */
        seq_arr[1] = 0;        /* a(1) = 0 */
        count_arr[0] = 1;      /* one occurrence of 0 so far */
        seq_max_n = 1;
    }

    /* ensure enough space for requested n */
    ensure_seq_capacity(&seq_cap, &seq_arr, n + 1);
    ensure_seq_capacity(&count_cap, &count_arr, n + 1);

    /* compute missing terms up to n */
    for (idx_t i = seq_max_n + 1; i <= n; ++i) {
        /* i == 1 cannot occur here because seq_max_n ≥ 1 */
        seq_t prev = seq_arr[i - 1];
        idx_t idx_prev = (idx_t)prev;
        seq_t k = count_arr[idx_prev];          /* count of prev among first i−1 terms */
        seq_t val = odd_part(k);                /* next term */
        seq_arr[i] = val;
        idx_t idx_val = (idx_t)val;
        ++count_arr[idx_val];                   /* update occurrence count of val */
    }

    seq_max_n = n;
    return seq_arr[n];
}
