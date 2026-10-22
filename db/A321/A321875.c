/*
 * A321875  a(n) = Sum_{d|n} d*d!.
 * https://oeis.org/A321875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*d!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* factorial cache: fact_cache[i] = i! */
static seq_t *fact_cache = NULL;   /* cached factorials */
static size_t fact_cap = 0;        /* allocated capacity of fact_cache */
static idx_t fact_mx = 0;          /* largest index for which factorial is cached */

/* ensure that factorials up to n are computed and stored */
static void ensure_factorials(idx_t n)
{
    if (n <= fact_mx)
        return;                       /* already available */

    /* grow capacity to at least n+1 */
    size_t new_cap = fact_cap ? fact_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                 /* double until big enough */

    void *tmp = realloc(fact_cache,
                        new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory */
    fact_cache = (seq_t *)tmp;
    fact_cap = new_cap;

    if (fact_mx == 0)                  /* initialise 0! */
        fact_cache[0] = (seq_t)1;

    /* compute missing factorials */
    for (idx_t i = fact_mx + 1; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    fact_mx = n;
}

/* the required sequence */
seq_t a(idx_t n)
{
    if (n == 0)                         /* safety: OEIS offset starts at 1 */
        return (seq_t)0;

    ensure_factorials(n);

    seq_t sum = (seq_t)0;

    /* loop over all proper divisor pairs d and n/d */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* add contribution of divisor d */
            sum += fact_cache[d] * (seq_t)d;

            idx_t d2 = n / d;           /* complementary divisor */
            if (d2 != d)                /* avoid double‑counting perfect square */
                sum += fact_cache[d2] * (seq_t)d2;
        }
    }
    return sum;
}
