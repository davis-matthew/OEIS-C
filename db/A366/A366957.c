/*
 * A366957  Expansion of e.g.f. 1/(1-x^2-3*x^3).
 * https://oeis.org/A366957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, and for n≥2, let c(n) be defined by c(0)=1,
 *            c(1)=0, c(2)=1, and c(n)=c(n‑2)+3·c(n‑3). Then a(n)=n!·c(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* factorial accumulator */
    seq_t fact = (seq_t)1;

    /* ring buffer holding the last three values of c() */
    seq_t cbuf[3];
    cbuf[0] = (seq_t)1; /* c(0) */
    cbuf[1] = (seq_t)0; /* c(1) */
    cbuf[2] = (seq_t)1; /* c(2) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* update factorial */
        fact = fact * (seq_t)i;

        seq_t ci;
        if (i == 2)
            ci = cbuf[2]; /* c(2) already set */
        else
        {
            /* c(i) = c(i‑2) + 3·c(i‑3) */
            ci = cbuf[(i - 2) % 3] + (seq_t)3 * cbuf[(i - 3) % 3];
        }
        cbuf[i % 3] = ci;
    }

    /* a(n) = n! * c(n) */
    return fact * cbuf[n % 3];
}
