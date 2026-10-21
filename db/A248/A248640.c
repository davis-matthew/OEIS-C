/*
 * A248640  Least nonnegative sequence which does not contain a 5-term
 *          equidistant subsequence (a(n+k*d); k=0,1,2,3,4) in arithmetic
 *          progression.
 * https://oeis.org/A248640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least non‑negative integer that, when appended to the
 *            sequence up to index n, does not create any 5‑term subsequence
 *            with equal arithmetic differences
 *            (a(i),a(i+d),a(i+2d),a(i+3d),a(i+4d))
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t   cache_cap  = 0;    /* total allocated slots          */
static size_t   cache_len  = 0;    /* number of terms actually computed */

/* Ensure that the cache can hold indices up to n inclusive */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                  /* out of memory */

    /* Initialise the newly added part of the array to zero. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure the cache can store the required indices. */
    ensure_cache(n);

    /* Compute any terms that have not yet been generated. */
    for (; cache_len <= (size_t)n; ++cache_len)
    {
        seq_t v = 0;
        while (true)
        {
            bool ok = true;

            /* Check every possible common difference d that
               yields an arithmetic progression ending at index
               cache_len with five terms. */
            for (idx_t d = 1; 4*d <= (idx_t)cache_len; ++d)
            {
                idx_t start = (idx_t)cache_len - 4*d;

                /* Retrieve the first four existing terms in the
                   potential progression. */
                seq_t a0 = seq_cache[start];
                seq_t a1 = seq_cache[start + d];
                seq_t a2 = seq_cache[start + 2*d];
                seq_t a3 = seq_cache[start + 3*d];

                /* Compute consecutive differences. */
                seq_t diff0 = a1 - a0;
                seq_t diff1 = a2 - a1;
                if (diff0 != diff1)
                    continue;               /* not an AP yet */

                seq_t diff2 = a3 - a2;
                if (diff1 != diff2)
                    continue;               /* not an AP yet */

                /* If the first three differences are equal,
                   check whether the candidate v would continue the
                   progression with the same difference. */
                seq_t diff3 = v - a3;
                if (diff3 == diff0) {
                    ok = false;              /* would create a 5-term AP */
                    break;
                }
            }

            if (ok)
                break;                      /* v is acceptable */

            ++v;                             /* try next candidate */
        }

        seq_cache[cache_len] = v;              /* store the term */
    }

    return seq_cache[(size_t)n];
}
