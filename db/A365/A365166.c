/*
 * A365166  Numbers that are not the sum of a prime number and a fourth power
 *          of a nonnegative integer.
 * https://oeis.org/A365166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365166: Numbers that are not the sum of a prime number and
   a fourth power of a non‑negative integer. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* Computes floor(n^(1/4)) for an arbitrary‑precision or fixed‑width
   integer value n. The result fits in an idx_t. */
static inline idx_t floor_fourth_root_seq(seq_t n)
{
    seq_t s  = floor_sqrt(n);   /* floor(sqrt(n))   */
    seq_t ss = floor_sqrt(s);   /* floor(sqrt(sqrt(n))) */
    return (idx_t)ss;           /* safe cast; ss is small */
}

/* Returns true iff m can be written as prime + k^4 (k ≥ 0). */
static bool _is_representable(seq_t m)
{
    const seq_t two = 2;
    idx_t kmax = floor_fourth_root_seq(m);
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t k2 = kseq * kseq;
        seq_t k4 = k2 * k2;
        seq_t p = m - k4;               /* p = m - k^4 */
        if (p < two) continue;          /* p must be at least 2 */
        if (is_prime_big(p))
            return true;
    }
    return false;
}

/* Cached sequence storage.  The array grows by doubling its
   capacity whenever it becomes full. */
static seq_t *cache      = NULL;   /* array of sequence values       */
static size_t cache_cap  = 0;      /* current allocated capacity     */
static size_t cache_len  = 0;      /* number of values stored        */
static idx_t last_checked = 0;    /* last integer examined          */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, but safe */

    /* initial allocation */
    if (!cache)
    {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure (unlikely) */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        ++last_checked;             /* start from 1 on first call   */

        if (!_is_representable(last_checked))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */

                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];             /* 0‑based indexing in the array */
}
