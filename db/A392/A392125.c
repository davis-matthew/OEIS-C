/*
 * A392125  Numbers k such that (t(k) - greatest prime < t(k)) > (- t(k) +
 *          least prime > t(k)), where t(k) = A000217(k).
 * https://oeis.org/A392125
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 63 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392125: Numbers k such that (t(k)-prev_prime<t(k)) > (next_prime>t(k)-t(k)),
   where t(k) = k*(k+1)/2.  The sequence is stored 1‑based with offset 2
   (a(2) is the first term).  The code builds the sequence lazily and memoises
   all found k values so that any call to a(n) is fast.  The routine assumes
   that the functions prev_prime and next_prime are available in
   "../utils/prev_prime.h" and "../utils/next_prime.h" respectively. */

/* Include needed headers */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

/* Return the n‑th term of A392125 (n >= 2). */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 2.  Return 0 for lower indices. */
    if (n < 2) return (seq_t)0;

    /* Static cache of already found k values */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;

    /* Next triangular number to test */
    static idx_t next_k = 2;

    /* Ensure we have enough entries for a(n).  required = n-1 because
       cache[0] holds the term for a(2). */
    size_t required = (size_t)(n - 1);
    while (cache_sz < required)
    {
        /* Compute triangular number t(k).  Use seq_t arithmetic. */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1) / 2;

        /* Convert to idx_t for prime lookup (safe for our range). */
        idx_t t_int = (idx_t)t;

        /* Find the largest prime < t(k). */
        idx_t prev_p = prev_prime(t_int);

        /* Find the smallest prime > t(k).  next_prime is assumed to
           return the next prime strictly greater than its argument. */
        idx_t next_p = next_prime(t_int);

        /* The inequalities t(k)-prev_p > next_p-t(k) are equivalent to
           2*t(k) > prev_p + next_p.  We use the gap form. */
        idx_t gap_prev = t_int - prev_p;
        idx_t gap_next = next_p - t_int;

        if (gap_prev > gap_next)
        {
            /* Cache the qualifying k.  Grow the array if needed. */
            if (cache_sz + 1 > cache_cap)
                ensure_capacity(cache_sz + 1, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
            cache[cache_sz++] = (seq_t)next_k;
        }

        /* Advance to the next k. */
        ++next_k;
    }

    /* cache[0] holds a(2), so a(n) is cache[n-2]. */
    size_t idx = (size_t)(n - 2);
    return cache[idx];
}
