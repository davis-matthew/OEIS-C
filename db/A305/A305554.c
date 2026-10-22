/*
 * A305554  Sum of the areas of all D-elevated skew paths of semi-length n.
 * https://oeis.org/A305554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A305554
 *  Sum of the areas of all D‑elevated skew paths of semi‑length n.
 *
 *  Formula (D‑finite recurrence, valid for n ≥ 4):
 *     a(n) = ((11*n-20)*a(n-1) + 5*(-7*n+19)*a(n-2) + 25*(n-4)*a(n-3))/(n-1)
 *
 *  Initial values:
 *     a(0)=0, a(1)=1, a(2)=4, a(3)=22
 *
 *  The recurrence uses only integer arithmetic; all intermediate
 *  values are non‑negative in the domain 0 ≤ n ≤ 10000.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cached = 0;   /* number of elements currently stored */

    /* extend cache if needed */
    if ((size_t)n + 1 > cached)
    {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp)          /* allocation failure – unlikely in tests */
            return (seq_t)0;
        cache = tmp;

        /* compute missing entries, starting after the last cached */
        for (size_t i = cached; i <= (size_t)n; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)0;
            else if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)4;
            else if (i == 3)
                cache[i] = (seq_t)22;
            else          /* recurrence for i ≥ 4 */
            {
                /* coefficients, computed as unsigned to avoid signed overflow */
                seq_t coeff1 = (seq_t)(((seq_t)11 * (seq_t)i) - (seq_t)20);
                seq_t fact   = (seq_t)(((seq_t)5) * ((seq_t)(7 * i - 19)));
                seq_t coeff3 = (seq_t)(((seq_t)25 * (seq_t)(i - 4)));

                seq_t t1 = coeff1 * cache[i-1];
                seq_t t2 = fact   * cache[i-2]; /* fact is positive (7*i‑19 ≥ 2) */
                seq_t t3 = coeff3 * cache[i-3];

                seq_t numer = t1 - t2 + t3;          /* (a(n-1)) * coeff1   */
                                                    /* minus (a(n-2)) * fact */
                cache[i]   = numer / (seq_t)(i-1);
            }
        }
        cached = newsize;
    }
    return cache[(size_t)n];
}
