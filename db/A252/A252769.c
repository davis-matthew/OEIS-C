/*
 * A252769  Numbers n such that the sum of the pentagonal numbers P(n),
 *          P(n+1), P(n+2) and P(n+3) is equal to the heptagonal number H(m)
 *          for some m.
 * https://oeis.org/A252769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A252769: Numbers n such that P(n)+P(n+1)+P(n+2)+P(n+3) is a heptagonal number.
   Recurrence: a(n) = 63*a(n-1) - 63*a(n-2) + a(n-3)   (n ≥ 4)
   Initial values: a(1)=94, a(2)=5908, a(3)=366282. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset and initial values explicitly. */
    if (n == 1) return 94;
    if (n == 2) return 5908;
    if (n == 3) return 366282;

    /* Variables holding the last three computed values, initially
       a(1), a(2), a(3).  Use seq_t to allow arbitrary‑precision when
       needed.  The recurrence never requires a subtraction that
       underflows since the sequence is strictly increasing. */
    seq_t a1 = 94;          /* a(n-3) */
    seq_t a2 = 5908;        /* a(n-2) */
    seq_t a3 = 366282;      /* a(n-1) */
    seq_t a4 = 0;           /* a(n) for current i */

    /* Compute terms from 4 up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        a4 = 63 * a3 - 63 * a2 + a1;  /* recurrence */
        a1 = a2;                      /* shift window */
        a2 = a3;
        a3 = a4;
    }

    return a3;  /* a(n) is now stored in a3 */
}
