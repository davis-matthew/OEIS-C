/*
 * A288270  E.g.f.: exp(Sum_{k>=1} (k-1)^2*x^k).
 * https://oeis.org/A288270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = (n-1)! * Sum_{k=1..n} (k-1)^2 * k *
 *            a(n-k)/(n-k)! for n > 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cap  = 0;
static idx_t seq_len  = 0;  /* number of entries computed, i.e. seq_len = last_index+1 */

/* Make sure the cache can hold values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : (size_t)n + 1;
    while (new_cap <= (size_t)n) new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap  = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;             /* a(0) = 1 */

    ensure_cache(n);                  /* ensure storage for 0..n */

    if (n >= seq_len)                 /* compute only new entries */
    {
        for (idx_t cur = seq_len; cur <= n; ++cur)
        {
            if (cur == 0)             /* a(0) */
            {
                seq_cache[0] = 1;
                continue;
            }

            seq_t sum = 0;
            if (cur >= 2)
            {
                seq_t ratio = (seq_t)(cur - 1);  /* for k = 2   (n-1)!/(n-2)! */
                for (idx_t k = 2; k <= cur; ++k)
                {
                    seq_t factor = (seq_t)k * (seq_t)(k - 1) * (seq_t)(k - 1);
                    seq_t term   = seq_cache[cur - k] * factor * ratio;
                    sum += term;
                    if (k < cur)          /* prepare ratio for next k */
                        ratio *= (seq_t)(cur - k);   /* (n-1)…(n-k) */
                }
            }
            seq_cache[cur] = sum;
        }
        seq_len = n + 1;               /* updated number of computed terms */
    }

    return seq_cache[n];
}
