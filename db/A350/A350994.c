/*
 * A350994  a(n) = (40*100^n + 6*10^n - 1)/3.
 * https://oeis.org/A350994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (40*100^n + 6*10^n - 1)/3
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=15; a(1)=1353; a(2)=133533;
   a(n) = 111*a(n‑1) – 1110*a(n‑2) + 1000*a(n‑3) for n≥3 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)15;
    if (n == 1) return (seq_t)1353;
    if (n == 2) return (seq_t)133533;

    /* Use a sliding window of three previous values. */
    seq_t a0 = (seq_t)15;          /* a(n‑3)  */
    seq_t a1 = (seq_t)1353;        /* a(n‑2)  */
    seq_t a2 = (seq_t)133533;      /* a(n‑1)  */
    seq_t an;                      /* current value */

    for (idx_t i = 3; i <= n; ++i) {
        an = 111 * a2 - 1110 * a1 + 1000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an; /* a2 holds a(n) after the loop */
}
