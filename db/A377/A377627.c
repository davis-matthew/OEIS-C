/*
 * A377627  Cogrowth sequence of the 12-element group C6 X C2 = <S,T | S^6,
 *          T^2, [S,T]>.
 * https://oeis.org/A377627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n, 6*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from generating function: for n ≥ 6, */
/* a(n) = 6*a(n-1) - 15*a(n-2) + 22*a(n-3) + 15*a(n-4) + 36*a(n-5). */

#include <stddef.h>

/* Base values a(0)..a(5) */
static const seq_t base[6] = { 1, 1, 1, 2, 29, 211 };

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n < 6)
        return base[n];

    /* Keep the last five terms to apply the recurrence */
    seq_t t1 = base[0]; /* a(n-5) */
    seq_t t2 = base[1]; /* a(n-4) */
    seq_t t3 = base[2]; /* a(n-3) */
    seq_t t4 = base[3]; /* a(n-2) */
    seq_t t5 = base[4]; /* a(n-1) */
    seq_t t6 = base[5]; /* a(n)   */

    /* Compute terms from 6 up to n */
    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 6*t6 - 15*t5 + 22*t4 + 15*t3 + 36*t2  */
        seq_t next = 6 * t6 - 15 * t5 + 22 * t4 + 15 * t3 + 36 * t2;

        /* Shift the window for the next iteration */
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = t6;
        t6 = next;
    }

    return t6;
}
