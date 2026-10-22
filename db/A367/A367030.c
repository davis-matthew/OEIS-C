/*
 * A367030  G.f. satisfies A(x) = 1 + x*A(x) - x^2*A(x)^5.
 * https://oeis.org/A367030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(n+3k,k) *
 *            binomial(n+2k,n-2k) / (4*k+1) * The recurrence used below
 *            updates the two binomial factors incrementally to avoid *
 *            recalculating them from scratch for each k
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 12, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    idx_t max_k = n / 2;
    seq_t sum = 0;
    seq_t B = 1;                 /* binom(n+3*0,0)   */
    seq_t C = 1;                 /* binom(n+2*0,n-0) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* term = (-1)^k * B * C / (4*k+1) */
        seq_t term = B * C;
        term /= (seq_t)(4 * k + 1);
        if (k & 1)
            term = -term;
        sum += term;

        if (k == max_k)
            break;

        /* update B and C for next k */
        idx_t k1 = k + 1;
        seq_t numB = (seq_t)(n + 3 * k1) * (seq_t)(n + 3 * k1 - 1) * (seq_t)(n + 3 * k1 - 2);
        seq_t denB = (seq_t)k1 * (seq_t)(n + 2 * k1) * (seq_t)(n + 2 * k1 - 1);
        B = B * numB / denB;

        seq_t numC = (seq_t)(n + 2 * k1 - 1) * (seq_t)(n + 2 * k1) *
                     (seq_t)(n - 2 * k1 + 1) * (seq_t)(n - 2 * k1 + 2);
        seq_t denC = (seq_t)(4 * k1 - 3) * (seq_t)(4 * k1 - 2) *
                     (seq_t)(4 * k1 - 1) * (seq_t)(4 * k1);
        C = C * numC / denC;
    }
    return sum;
}
