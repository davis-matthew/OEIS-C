/*
 * A387367  Expansion of 1/(1 - 10*x + x^2)^(5/2).
 * https://oeis.org/A387367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for OEIS A387367: 
 *    a(0) = 1, a(1) = 25
 *    a(n) = (5*(2*n+3)*a(n-1) - (n+3)*a(n-2)) / n   for n > 1
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)25;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)25;  /* a(1) */
    seq_t an = a1;        /* holds a(i) */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* compute a(i) from a1=a(i-1) and a0=a(i-2) */
        if (sizeof(seq_t) <= 8)
        {
            /* use 128‑bit intermediate to avoid overflow on 64‑bit compilers */
            unsigned __int128 num = (unsigned __int128)5 * (unsigned __int128)(2*i+3) * (unsigned __int128)a1;
            num -= (unsigned __int128)(i+3) * (unsigned __int128)a0;
            an = (seq_t)(num / i);
        }
        else
        {
            /* arbitrary‑precision or >64‑bit type: operations are exact */
            an = ((seq_t)5 * (seq_t)(2*i+3) * a1
                  - (seq_t)(i+3) * a0) / (seq_t)i;
        }

        /* shift for next step */
        a0 = a1;
        a1 = an;
    }

    return an;
}
