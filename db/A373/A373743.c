/*
 * A373743  Expansion of e.g.f. exp(x^3/6 * (1 + x)^2).
 * https://oeis.org/A373743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑1)*(n‑2)/6 * (3*a(n‑3) + 8*(n‑3)*a(n‑4) +
 *            5*(n‑3)*(n‑4)*a(n‑5)), a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=8
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the trivial starting values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)8;

    /* Small circular buffer holding the last six computed terms. */
    seq_t buf[6] = {(seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)8, (seq_t)0};

    /* Compute terms from 5 up to n. */
    for (idx_t i = 5; i <= n; ++i)
    {
        idx_t im3 = (i - 3) % 6;
        idx_t im4 = (i - 4) % 6;
        idx_t im5 = (i - 5) % 6;

        seq_t sub = 3 * buf[im3]
                  + 8 * (idx_t)(i - 3) * buf[im4]
                  + 5 * (idx_t)(i - 3) * (idx_t)(i - 4) * buf[im5];

        seq_t term = (idx_t)(i - 1) * (i - 2) * sub / 6;
        buf[i % 6] = term;
    }

    return buf[n % 6];
}
