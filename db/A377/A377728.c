/*
 * A377728  Convolution of Leonardo numbers with Jacobsthal numbers.
 * https://oeis.org/A377728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377728
   Convolution of Leonardo numbers with Jacobsthal numbers
   Recurrence (n ≥ 5):
     a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5)
   Initial values:
     a(0)=0, a(1)=1, a(2)=2, a(3)=7, a(4)=16
   This recurrence follows from the generating function
   G(x) = -x*(x^2-x+1)/((x-1)*(2x-1)*(x+1)*(x^2+x-1)).
*/

#include <stdlib.h>

static seq_t *a_cache   = NULL; /* cached sequence values */
static size_t a_cap     = 0;    /* capacity of a_cache */
static size_t a_last    = 0;    /* highest index cached (inclusive) */

/* ensure a_cache of at least 'needed+1' elements, reallocating if necessary */
static void ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed < *cap) return;
    size_t new_cap = (*cap) ? (*cap << 1) : 4;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* initialize cache with the first 5 terms of the sequence */
static void init_a_cache(void)
{
    if (a_cache) return;
    size_t init_cap = 8;
    a_cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
    if (!a_cache) abort();
    a_cap   = init_cap;
    a_cache[0] = (seq_t)0;
    a_cache[1] = (seq_t)1;
    a_cache[2] = (seq_t)2;
    a_cache[3] = (seq_t)7;
    a_cache[4] = (seq_t)16;
    a_last = 4;
}

/* compute a(n) for all n up to idx if not already cached */
static void ensure_a(size_t idx)
{
    init_a_cache();
    if (idx <= a_last) return;
    ensure_capacity(idx, &a_cache, &a_cap);
    for (size_t i = a_last + 1; i <= idx; ++i)
    {
        /* recurrence: a(i) = 3*a(i-1) - 5*a(i-3) + a(i-4) + 2*a(i-5) */
        seq_t term = a_cache[i - 1] * (seq_t)3;
        term += a_cache[i - 4];
        term += a_cache[i - 5] * (seq_t)2;
        term -= a_cache[i - 3] * (seq_t)5;
        a_cache[i] = term;
    }
    a_last = idx;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_a(idx);
    return a_cache[idx];
}
