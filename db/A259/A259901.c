/*
 * A259901  n*a(n+1) = (2*n^2+3*n+2)*a(n)-(n^2-n-2)*a(n-1); a(0)=1, a(1)=0.
 * https://oeis.org/A259901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   n*a(n+1) = (2*n^2+3*n+2)*a(n) - (n^2-n-2)*a(n-1), a(0) = 1, a(1)
 *            = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_prev = (seq_t)1; /* a(0) */
    seq_t a_cur  = (seq_t)0; /* a(1) */

    for (idx_t i = 1; i <= n-1; ++i)
    {
        /* coefficients, i is at most 10000 in the relevant tests */
        unsigned long long coeff1 = 2ULL * i * i + 3ULL * i + 2ULL;
        long long          coeff2 = (long long)i * (long long)i - (long long)i - 2LL;

        /* products – all positive because coeff2 is handled separately */
        seq_t prod1 = coeff1 * a_cur;
        seq_t prod2;
        seq_t numer;

        if (coeff2 >= 0)
        {
            prod2 = (seq_t)coeff2 * a_prev;    /* coeff2 is non‑negative */
            numer = prod1 - prod2;
        }
        else
        {
            prod2 = (seq_t)(-coeff2) * a_prev; /* add because coeff2 is negative */
            numer = prod1 + prod2;
        }

        /* division by i (i > 0) – guaranteed exact by the recurrence */
        seq_t a_next = numer / (seq_t)i;

        a_prev = a_cur;
        a_cur  = a_next;
    }

    return a_cur;   /* a(n) */
}
