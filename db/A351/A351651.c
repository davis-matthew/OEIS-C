/*
 * A351651  a(n) is the quotient obtained when digsum(m^2) is divided by
 *          digsum(m), with digsum = sum of digits = A007953 and m =
 *          A351650(n).
 * https://oeis.org/A351651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004159(A351650(n)) / A007953(A351650(n))
 * Range:     u8 n <= 8, u16 n <= 82, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Lazy memoization of the quotients for indices 1..n */
    static seq_t *quotients = NULL;
    static size_t cap = 0;
    static size_t size = 0;          /* number of quotients stored */
    static uint64_t last_m = 0;      /* last m processed */

    if (n == 0) return 0;            /* undefined for n<1, but guard anyway */

    /* Generate quotients until we have at least n of them */
    while (size < (size_t)n)
    {
        last_m++;
        seq_t m = (seq_t)last_m;
        seq_t m_sq = m * m;          /* m squared fits in seq_t for the range of interest */

        seq_t dg_m  = digit_sum(m);
        seq_t dg_sq = digit_sum(m_sq);

        if (dg_m != 0 && dg_sq % dg_m == 0)
        {
            seq_t val = dg_sq / dg_m;
            if (size == cap)
            {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(quotients, newcap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure; caller may handle */
                quotients = tmp;
                cap = newcap;
            }
            quotients[size++] = val;
        }
    }

    return quotients[n - 1];
}
