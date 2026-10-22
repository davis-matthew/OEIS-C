/*
 * A362029  a(n) = Sum_{k=1..n} (-1)^(n-k) * k * mu(k)^2, where mu(k) is the
 *          Moebius function.
 * https://oeis.org/A362029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * k * mu(k)^2
 * Range:     i8 n <= 36, i16 n <= 572, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Dynamic cache of previously computed terms */
    static seq_t *term = NULL;          /* array of a(1) ... a(len) */
    static idx_t len = 0;               /* number of terms stored */

    /* Extend cache if necessary */
    if (n > len)
    {
        /* Allocate or grow the array to hold n terms */
        term = (seq_t *)realloc(term, (size_t)n * sizeof(seq_t));

        /* Compute new terms using the recurrence
           a(n) = -a(n-1) + f(n), where f(n) = n if n is squarefree,
           otherwise f(n) = 0 (equivalent to n*mu(n)^2). */
        for (idx_t i = len + 1; i <= n; ++i)
        {
            seq_t mu = mobius(i);                      /* μ(i) */
            seq_t f  = (mu == 0) ? (seq_t)0 : (seq_t)i; /* i*μ(i)^2 */

            if (i == 1)
                term[i - 1] = f;            /* a(1) = f(1) = 1 */
            else
                term[i - 1] = -term[i - 2] + f; /* recurrence */
        }
        len = n;
    }

    return term[n - 1];
}
