/*
 * A396464  a(1) = a(2) = 1. For even n, a(n) = a(n-1) + a(n-2). For odd n,
 *          a(n) = a(n-1) divided by the largest proper factor found in the
 *          previous terms. If no proper factor is present, then a(n) = a(n-1)
 *          + a(n-2).
 * https://oeis.org/A396464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396464:
   a(1) = a(2) = 1.
   For even n: a(n) = a(n-1) + a(n-2).
   For odd n: let d be the largest *proper* divisor (greater than 1 and
   less than a(n‑1)) of a(n-1) that appears among a(1)…a(n-2).
   If such a d exists, a(n) = a(n-1) / d; otherwise a(n) = a(n-1) + a(n-2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic cache storing computed terms.
   cache[i] corresponds to a(i+1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of stored terms          */
static idx_t cache_cap = 0;   /* allocated capacity              */

static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap < need) {
        if (new_cap > SIZE_MAX / 2)
            new_cap = need;
        else
            new_cap <<= 1;
    }
    seq_t *newp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!newp)
        abort();  /* allocation failure */
    cache     = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined (offset starts at 1) */

    /* Grow cache up to n terms. */
    if (n > cache_len) {
        ensure_cache(n);

        for (idx_t i = cache_len; i < n; ++i) {   /* i = 0‑based index */
            idx_t idx = i + 1;                    /* 1‑based index */
            seq_t val;

            if (idx == 1 || idx == 2)
                val = (seq_t)1;
            else if ((idx & 1u) == 0) {            /* even */
                val = cache[i - 1] + cache[i - 2];
            } else {                               /* odd */
                seq_t prev  = cache[i - 1];
                seq_t prev2 = cache[i - 2];

                seq_t max_div = 0;
                /* Search all previous terms (a(1)…a(idx‑2)). */
                for (idx_t j = 0; j < i - 1; ++j) {
                    seq_t cand = cache[j];
                    if (cand > (seq_t)1 && cand < prev &&
                        (prev % cand) == 0) {
                        if (cand > max_div)
                            max_div = cand;
                    }
                }

                if (max_div > 0)
                    val = prev / max_div;
                else
                    val = prev + prev2;
            }

            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
