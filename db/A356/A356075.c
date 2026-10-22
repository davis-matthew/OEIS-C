/*
 * A356075  a(n) = floor(m), where li(m) = n and li is the logarithmic
 *          integral. We consider only real and positive m.
 * https://oeis.org/A356075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(m) where li(m) = n (Cauchy principal value)
 * Range:     u8 n <= 60, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   ei, li_of_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ei.h"
#include "../utils/li_of_m.h"

/* Static cache of li(m) values.  li_cache[i] is li(i).  */
static double *li_cache = NULL;          /* dynamic array */
static idx_t li_len   = 0;               /* number of entries allocated (0 … li_len-1) */

/* Ensure that li_cache has entries up to index m inclusive.  */
static void ensure_li_upto(idx_t m)
{
    while (li_len <= m) {
        idx_t new_len = li_len ? li_len * 2 : 16;
        if (new_len <= m) new_len = m + 1;
        double *tmp = (double *)realloc(li_cache, new_len * sizeof(double));
        if (!tmp) abort();               /* allocation failed */
        li_cache = tmp;
        for (idx_t i = li_len; i < new_len; ++i) {
            if (i < 2)
                li_cache[i] = -1.0;      /* unused slots */
            else
                li_cache[i] = li_of_m(i);
        }
        li_len = new_len;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;  /* special case, avoids li(1) */

    /* Rough upper bound for the integer part of the inverse li.  */
    idx_t m_est = (idx_t)(n * (log((double)n) + log(log((double)n)) - 1.0) + 20.0);
    if (m_est < 2) m_est = 2;

    ensure_li_upto(m_est);
    /* Grow cache until it covers all li values up to n.  */
    while (li_cache[li_len - 1] < (double)n) {
        ensure_li_upto(li_len * 2);
    }

    /* Binary search for the largest m with li(m) <= n. */
    idx_t low = 2;
    idx_t high = li_len - 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (li_cache[mid] <= (double)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    /* high now holds the desired floor(m).  */
    return (seq_t)high;
}
