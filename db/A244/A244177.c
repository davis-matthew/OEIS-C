/*
 * A244177  Number of length n+4 0..2 arrays with no five consecutive elements
 *          with pattern ababa or abbba (possibly a=b) and new values 0..2
 *          introduced in 0..2 order.
 * https://oeis.org/A244177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244177: Number of length n+4 0..2 arrays with no five consecutive
 * elements with pattern ababa or abbba (possibly a=b) and new values 0..2
 * introduced in 0..2 order.
 *
 * Recurrence (for n ≥ 7):
 *   a(n) = a(n‑1) + 3·a(n‑2) + 4·a(n‑3) + 5·a(n‑4) + 3·a(n‑5) + 2·a(n‑6)
 * Initial values:
 *   a(1) = 38,  a(2) = 108,  a(3) = 306,  a(4) = 867,
 *   a(5) = 2457, a(6) = 6963
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Return immediately for n <= 6 using the known initial values. */
    if (n == 1) return (seq_t)38;
    if (n == 2) return (seq_t)108;
    if (n == 3) return (seq_t)306;
    if (n == 4) return (seq_t)867;
    if (n == 5) return (seq_t)2457;
    if (n == 6) return (seq_t)6963;

    /* a1 … a6 hold the six previous terms a(n‑6) … a(n‑1).  We start
     * with the initial values for a(n‑6) … a(n‑1) at n = 7. */
    seq_t a1 = (seq_t)38;   /* a(n‑6) when n = 7 */
    seq_t a2 = (seq_t)108;  /* a(n‑5) */
    seq_t a3 = (seq_t)306;  /* a(n‑4) */
    seq_t a4 = (seq_t)867;  /* a(n‑3) */
    seq_t a5 = (seq_t)2457; /* a(n‑2) */
    seq_t a6 = (seq_t)6963; /* a(n‑1) */

    seq_t newterm;
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = a6 + 3*a5 + 4*a4 + 5*a3 + 3*a2 + 2*a1 */
        newterm = a6
                + (seq_t)3 * a5
                + (seq_t)4 * a4
                + (seq_t)5 * a3
                + (seq_t)3 * a2
                + (seq_t)2 * a1;

        /* Shift the window: a1 becomes a2, ..., a6 becomes the new term. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = newterm;
    }

    return a6;  /* a(n) is stored in a6 after the loop */
}
