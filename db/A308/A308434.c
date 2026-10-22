/*
 * A308434  a(n) = n! + n!!.
 * https://oeis.org/A308434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + n!!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Caches for factorials and double factorials */
    static seq_t *fact   = NULL;
    static seq_t *dfact  = NULL;
    static size_t fact_cap  = 0;
    static size_t dfact_cap = 0;
    size_t i, limit;

    /* Ensure factorial cache is large enough */
    if (fact_cap <= (size_t)n) {
        limit = (size_t)n + 1;
        fact = (seq_t *)realloc(fact, limit * sizeof(seq_t));
        for (i = fact_cap; i < limit; ++i) {
            if (i == 0)
                fact[i] = 1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = limit;
    }

    /* Ensure double‑factorial cache is large enough */
    if (dfact_cap <= (size_t)n) {
        limit = (size_t)n + 1;
        dfact = (seq_t *)realloc(dfact, limit * sizeof(seq_t));
        for (i = dfact_cap; i < limit; ++i) {
            if (i <= 1)
                dfact[i] = 1;
            else
                dfact[i] = dfact[i - 2] * (seq_t)i;
        }
        dfact_cap = limit;
    }

    /* Return the sum of n! and n!! */
    return fact[(size_t)n] + dfact[(size_t)n];
}
