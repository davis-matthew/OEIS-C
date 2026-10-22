/*
 * A346505  G.f. A(x) satisfies: A(x) = (1 + x * A(x)^2) / (1 - x + 2 * x^2).
 * https://oeis.org/A346505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 2
 * a(n) = 2*a(n-1) + Σ_{k=2}^{n-1} a(k)*a(n-k-1)   for n ≥ 2   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cache of computed terms */
    static idx_t size = 0;     /* number of pre‑computed terms (seq[0..size-1]) */

    /* ensure cache large enough for requested n */
    if (n >= size)
    {
        /* allocate or grow the cache */
        size_t new_size = (size == 0 ? 2 : n + 1);
        seq_t *tmp = (seq_t *)realloc(seq, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;   /* allocation failure – caller will not use this value */

        seq = tmp;

        /* initialise base cases if not yet initialised */
        if (size == 0)
        {
            seq[0] = (seq_t)1;
            seq[1] = (seq_t)2;
            size = 2;
        }

        /* compute new terms up to n */
        for (idx_t i = size; i <= n; ++i)
        {
            seq_t sum = 0;
            for (idx_t k = 2; k <= i - 1; ++k)
            {
                sum += seq[k] * seq[i - k - 1];
            }
            seq[i] = (seq_t)2 * seq[i - 1] + sum;
        }
        size = n + 1;
    }

    return seq[n];
}
