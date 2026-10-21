/*
 * A270084  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 65", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 10*2^n + 12 (n > 1) Initial values: a(0) = 1,
 *            a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* N.B.  pow2 and pow4 helpers are provided in utils. */
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* For n > 1 use the closed‑form expression */
    seq_t p2 = pow2(n);      /* 2^n  */
    seq_t p4 = pow4(n);      /* 4^n  */

    /* 4*4^n - 10*2^n + 12  */
    seq_t result = (seq_t)4 * p4;
    result -= (seq_t)10 * p2;
    result += (seq_t)12;

    return result;
}
