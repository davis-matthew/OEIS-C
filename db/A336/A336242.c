/*
 * A336242  a(n) = (n!)^2 * Sum_{d|n} (-1)^(d+1) / (d!)^2.
 * https://oeis.org/A336242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{d|n} (-1)^(d+1) / (d!)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached factorials: fact_cache[i] = i!  (i = 0 .. fact_cache_len-1) */
static seq_t *fact_cache = NULL;
static size_t fact_cache_len = 0;

/* Ensure the factorial cache contains the factorials up to n. */
static void ensure_fact_cache(idx_t n)
{
    /* Allocate at least n+1 entries */
    if ((size_t)n + 1 <= fact_cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* allocation failure */

    /* Copy old data if any */
    for (size_t i = 0; i < fact_cache_len; ++i)
        new_cache[i] = fact_cache[i];

    /* Compute missing factorials */
    if (fact_cache_len == 0) {          /* no old data */
        new_cache[0] = 1;               /* 0! = 1 */
        for (size_t i = 1; i < new_len; ++i)
            new_cache[i] = new_cache[i - 1] * (seq_t)i;
    } else {
        for (size_t i = fact_cache_len; i < new_len; ++i)
            new_cache[i] = new_cache[i - 1] * (seq_t)i;
    }

    /* Replace old cache */
    free(fact_cache);
    fact_cache     = new_cache;
    fact_cache_len = new_len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1 */

    /* Make sure we have all factorials up to n */
    ensure_fact_cache(n);

    const seq_t fact_n = fact_cache[(size_t)n];

    /* Sum over all divisors of n */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0)
            continue;

        const seq_t fact_d = fact_cache[(size_t)d];
        /* (n! / d!)^2 */
        seq_t ratio = fact_n / fact_d;          /* integer division, exact */
        seq_t term  = ratio * ratio;

        if ((d & 1) == 0)                       /* even d → -1 */
            sum_neg += term;
        else                                    /* odd d  → +1 */
            sum_pos += term;
    }

    /* Final result: sum over odd divisors minus sum over even divisors */
    return sum_pos - sum_neg;
}
