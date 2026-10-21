/*
 * A250849  Number of (n+1) X (4+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  A250849 : Number of (n+1)×5 arrays with the given monotonic
 *  conditions. The sequence satisfies the linear recurrence
 *
 *    a(n) = 10·a(n‑1) – 35·a(n‑2) + 50·a(n‑3) – 24·a(n‑4)
 *
 *  with initial values
 *
 *    a(1) = 3136
 *    a(2) = 21436
 *    a(3) = 123016
 *    a(4) = 626416
 *
 *  The implementation below evaluates the recurrence in O(n)
 *  time and uses only the basic arithmetic allowed for seq_t.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 0;            /* not part of OEIS offset, but harmless */
    if (n == 1) return (seq_t)3136;
    if (n == 2) return (seq_t)21436;
    if (n == 3) return (seq_t)123016;
    if (n == 4) return (seq_t)626416;

    /* Variables holding the four preceding values. */
    seq_t a_prev4 = (seq_t)3136;     /* a(n‑4)  */
    seq_t a_prev3 = (seq_t)21436;    /* a(n‑3)  */
    seq_t a_prev2 = (seq_t)123016;   /* a(n‑2)  */
    seq_t a_prev1 = (seq_t)626416;   /* a(n‑1)  */
    seq_t a_curr   = 0;

    /* Iterate the recurrence up to the requested n. */
    for (idx_t k = 5; k <= n; ++k) {
        a_curr = 10 * a_prev1 + 50 * a_prev3;   /* 10*a(n-1) + 50*a(n-3) */
        a_curr -= 35 * a_prev2;                 /* - 35*a(n-2)          */
        a_curr -= 24 * a_prev4;                 /* - 24*a(n-4)          */

        /* Shift the window of preceding terms. */
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
