/*
 * A325846  First term of n-th difference sequence of round((k*Pi)), k >= 0.
 * https://oeis.org/A325846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = nth finite difference at k=0 of the sequence round(k*Pi),
 *            where round(x) = floor(x + 0.5). This is implemented using the
 *            rational approximation Pi ≈ 355/113, which is accurate enough
 *            for all 0 ≤ k ≤ 10000, so round(k*Pi) = floor((k*355 + 56)/113)
 * Range:     i8 none, i16 n <= 18, i32 n <= 35, i64 n <= 70, i128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* offset is 1, but return 0 for safety */

    /* Store the values of round(k*Pi) for k = 0 … n. */
    size_t len = (size_t)(n + 1);
    seq_t *v = (seq_t *)malloc(len * sizeof(seq_t));
    if (!v)
        return (seq_t)0;   /* allocation failed – unlikely test case */

    /* Compute the initial sequence */
    for (size_t k = 0; k < len; ++k)
        v[k] = (((seq_t)k) * (seq_t)355 + (seq_t)56) / (seq_t)113;

    /* Apply the difference operator n times.  After each level the
       length of the usable prefix decreases by one. */
    for (idx_t level = 1; level <= n; ++level)
    {
        size_t newlen = len - (size_t)level;
        for (size_t i = 0; i < newlen; ++i)
            v[i] = v[i + 1] - v[i];
    }

    seq_t result = v[0];
    free(v);
    return result;
}
