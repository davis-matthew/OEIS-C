/*
 * A293117  E.g.f.: exp(x^2/(x-1)).
 * https://oeis.org/A293117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2*n-2)*a(n-1) - (n^2-n)*a(n-2) + (n^2-3*n+2)*a(n-3)
 *            Initial values: a(0) = 1 a(1) = 0 a(2) = -2
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t if needed */

seq_t a(idx_t n)
{
    /* handle initial values explicitly */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)0;
    if (n == 2U) return (seq_t)-2;

    /* we keep only the last three values to save memory */
    seq_t a0 = (seq_t)1;           /* a(n-3) */
    seq_t a1 = (seq_t)0;           /* a(n-2) */
    seq_t a2 = (seq_t)-2;          /* a(n-1) */

    /* iterate from 3 up to n */
    for (idx_t i = 3U; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t two_i_minus_two   = (seq_t)2 * i_seq - (seq_t)2;
        seq_t i_sq_minus_i      = i_seq * i_seq - i_seq;
        seq_t i_sq_minus_3i_plus_2 = i_seq * i_seq - (seq_t)3 * i_seq + (seq_t)2;

        seq_t ai = two_i_minus_two * a2
                 - i_sq_minus_i * a1
                 + i_sq_minus_3i_plus_2 * a0;

        /* shift window: a(n-3), a(n-2), a(n-1) -> a(n-2), a(n-1), a(n) */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
