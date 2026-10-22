/*
 * A396093  G.f. A(x) = B(B(B(x))), the third iteration of B(x) = x/(1-x)^2.
 * https://oeis.org/A396093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in x*(1-x)^2*(1-3x+x^2)^2 /
 *            (1-7x+13x^2-7x^3+x^4)^2 (i.e., third iterate of B(x)=x/(1-x)^2).
 *            The coefficients satisfy the linear recurrence a(n) = 14*a(n-1)
 *            - 75*a(n-2) + 196*a(n-3) - 269*a(n-4) + 196*a(n-5) - 75*a(n-6) +
 *            14*a(n-7) - a(n-8) for n >= 8. The first eight va...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Coefficients of the denominator polynomial P(x) = (1-7x+13x^2-7x^3+x^4)^2 */
    static const int C[9] = {1, -14, 75, -196, 269, -196, 75, -14, 1};

    /* Numerator coefficients N(x) = x - 8x^2 + 24x^3 - 34x^4 + 24x^5 - 8x^6 + x^7 */
    static const int N[8] = {0, 1, -8, 24, -34, 24, -8, 1};

    /* Compute first eight terms a(0)…a(7). */
    seq_t first[8];
    for (idx_t i = 0; i <= 7 && i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* Handle numerator coefficient N[i] */
        if (N[i] > 0)   pos += (seq_t)N[i];
        else            neg += (seq_t)(-N[i]);

        /* Contribiutions from previous values */
        for (idx_t k = 1; k <= i; ++k) {
            int mult = -C[k];                 /* -C[k] = 14, -75, 196, …              */
            seq_t prod = first[i - k] * (seq_t)(mult > 0 ? mult : -mult);
            if (mult > 0)   pos += prod;
            else            neg += prod;      /* mult < 0 → contributes to 'neg' */
        }
        first[i] = pos - neg;
    }
    if (n <= 7) return first[(size_t)n];

    /* Use a 8‑element ring buffer for the last eight values. */
    seq_t buf[8] = {
        first[0], first[1], first[2], first[3],
        first[4], first[5], first[6], first[7]
    };

    seq_t next;
    for (idx_t i = 8; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* Positive contributions (coefficients 14, 196, 196, 14) */
        pos += buf[7] * (seq_t)14;   /* a(i-1) */
        pos += buf[5] * (seq_t)196;  /* a(i-3) */
        pos += buf[3] * (seq_t)196;  /* a(i-5) */
        pos += buf[1] * (seq_t)14;   /* a(i-7) */

        /* Negative contributions (coefficients 75, 269, 75, 1) */
        neg += buf[6] * (seq_t)75;   /* a(i-2) */
        neg += buf[4] * (seq_t)269;  /* a(i-4) */
        neg += buf[2] * (seq_t)75;   /* a(i-6) */
        neg += buf[0];               /* a(i-8) */

        next = pos - neg;   /* guaranteed non‑negative by the recurrence */

        /* Shift buffer left and insert new value */
        for (int j = 0; j < 7; ++j) buf[j] = buf[j + 1];
        buf[7] = next;
    }
    return buf[7];
}
