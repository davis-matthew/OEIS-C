/*
 * A393095  Group the partitions of n into 3 parts into sets whose smallest
 *          parts are equal. a(n) is the sum of the minimum values of all the
 *          largest parts in each set.
 * https://oeis.org/A393095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..⌊n/3⌋} ceil((n-j)/2) Recurrence: a(n) = a(n‑2)
 *            + 2*a(n‑3) – 2*a(n‑5) – a(n‑6) + a(n‑8), with a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=2, a(5)=2, a(6)=5, a(7)=6
 * Range:     u8 n <= 43, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the value for index n using the sum formula (used for n<8). */
static seq_t sum_formula(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n / 3;                 /* floor(n/3) */
    for (idx_t j = 1; j <= m; ++j) {
        /* ceil((n-j)/2) = ((n-j)+1)/2 for integer arithmetic */
        seq_t term = (seq_t)((n - j) + 1) / 2;
        sum += term;
    }
    return sum;
}

static seq_t *cache = NULL;          /* cache of computed values */
static idx_t cache_cap = 0;          /* capacity of cache array */

/* Ensure that cache has capacity for indices up to max_n inclusive. */
static void ensure_cache(idx_t max_n)
{
    if (cache_cap > max_n)
        return;

    idx_t new_cap = max_n + 1;
    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (!new_cache) {
        /* In a contest setting we would handle allocation failure; here
           we simply abort. */
        abort();
    }

    /* Copy existing values if cache already existed */
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
    }

    /* Initialize new entries */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        if (i <= 7) {
            new_cache[i] = sum_formula(i);
        } else {
            /* Recurrence: a(i) = a(i-2) + 2*a(i-3) – 2*a(i-5) – a(i-6) + a(i-8) */
            seq_t t = new_cache[i - 2];
            seq_t tmp = new_cache[i - 3];
            tmp += tmp;                    /* 2*a(i-3) */
            t += tmp;
            t += new_cache[i - 8];
            tmp = new_cache[i - 5];
            tmp += tmp;                    /* 2*a(i-5) */
            t -= tmp;
            t -= new_cache[i - 6];
            new_cache[i] = t;
        }
    }

    mem_free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)(-1))
        return 0;  /* undefined index, return 0 */

    ensure_cache(n);
    return cache[n];
}
