/*
 * A276644  Self-composition of the repunits; g.f.: A(x) = G(G(x)), where G(x)
 *          = g.f. of A002275.
 * https://oeis.org/A276644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*a(n‑1) – 272*a(n‑2) + 330*a(n‑3) – 100*a(n‑4) (n>3) *
 *            Recurrence initial values: a(0)=0, a(1)=1, a(2)=22, a(3)=464
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)464;

    /* use local variables to iteratively compute the sequence */
    seq_t a0 = (seq_t)0;   /* a(0)  */
    seq_t a1 = (seq_t)1;   /* a(1)  */
    seq_t a2 = (seq_t)22;  /* a(2)  */
    seq_t a3 = (seq_t)464; /* a(3)  */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)33 * a3
            - (seq_t)272 * a2
            + (seq_t)330 * a1
            - (seq_t)100 * a0;
        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
