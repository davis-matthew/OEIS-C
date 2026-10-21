/*
 * A261038  a(1)=1; for n>1: a(n) = a(n-1)*n if t=0, a(n) = round(a(n-1)/n) if
 *          t=1, a(n) = a(n-1)+n if t=2, a(n) = a(n-1)-n if t=3, where t = n
 *          mod 4.
 * https://oeis.org/A261038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 367, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) defined by the recurrence:
   a(1) = 1;
   for n > 1:
     let t = n mod 4
     if t == 0: a(n) = a(n‑1) * n
     if t == 1: a(n) = round(a(n‑1) / n)
     if t == 2: a(n) = a(n‑1) + n
     if t == 3: a(n) = a(n‑1) - n
   (offset 1 – a(1) is the first term) */

#include <stdlib.h>

/* Cached values of the sequence.  cache[i] holds a(i+1). */
static seq_t *cache      = NULL;   /* pointer to allocated array */
static size_t cache_cap  = 0;      /* current allocation size (number of elements) */
static idx_t  cache_len  = 0;      /* number of terms that have been computed */

/* Make sure the cache can hold at least `needed` elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if ((size_t)needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < (size_t)needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined but harmless for n=0 */

    /* initialise first term if necessary */
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* If the requested term is already computed, return it. */
    if (n <= cache_len) return cache[n-1];

    /* Make sure the cache can hold the new terms. */
    ensure_cache_capacity(n);

    /* Compute missing terms in order. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i-2];      /* a(i‑1) */

        idx_t mod = i & 3;            /* i mod 4 */

        seq_t val;
        switch (mod) {
            case 0:                /* multiplication */
                val = prev * (seq_t)i;
                break;
            case 1: {              /* round division (nearest integer, ties away from zero) */
                seq_t q = prev / (seq_t)i;
                seq_t r = prev % (seq_t)i;
                if (r != 0) {
                    /* absolute value of remainder */
                    seq_t abs_r = r >= 0 ? r : -r;
                    if (abs_r * 2 >= (seq_t)i) {
                        q += (r > 0) ? 1 : -1;
                    }
                }
                val = q;
            } break;
            case 2:                /* addition */
                val = prev + (seq_t)i;
                break;
            case 3:                /* subtraction */
                val = prev - (seq_t)i;
                break;
            default:
                val = prev;  /* unreachable */
                break;
        }

        cache[i-1] = val;   /* store a(i) */
    }

    cache_len = n;
    return cache[n-1];
}
