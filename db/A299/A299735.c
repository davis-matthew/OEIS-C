/*
 * A299735  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4, 5 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n‑1) –4*a(n‑2) –13*a(n‑3) –6*a(n‑4) +5*a(n‑5)
 *            +23*a(n‑6) –20*a(n‑7) –12*a(n‑8) Initial values (from the OEIS
 *            entry): a(1)=4, a(2)=32, a(3)=220, a(4)=1578, a(5)=11303,
 *            a(6)=81105, a(7)=582032, a(8)=4177161
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1, but return 0 */
        return 0;

    /* handle the first eight terms explicitly */
    if (n == 1) return 4;
    if (n == 2) return 32;
    if (n == 3) return 220;
    if (n == 4) return 1578;
    if (n == 5) return 11303;
    if (n == 6) return 81105;
    if (n == 7) return 582032;
    if (n == 8) return 4177161;

    /* iterate the recurrence; only keep the last eight values */
    seq_t a1 = 4;          /* a(n-8) */
    seq_t a2 = 32;         /* a(n-7) */
    seq_t a3 = 220;        /* a(n-6) */
    seq_t a4 = 1578;       /* a(n-5) */
    seq_t a5 = 11303;      /* a(n-4) */
    seq_t a6 = 81105;      /* a(n-3) */
    seq_t a7 = 582032;     /* a(n-2) */
    seq_t a8 = 4177161;    /* a(n-1) */
    seq_t ai;

    for (idx_t i = 9; i <= n; ++i) {
        ai = 8 * a8 - 4 * a7 - 13 * a6 - 6 * a5 + 5 * a4
           + 23 * a3 - 20 * a2 - 12 * a1;
        /* shift the window */
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = a7; a7 = a8; a8 = ai;
    }
    return a8;
}
