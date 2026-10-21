/*
 * A298210  Smallest n such that A001542(a(n)) == 0 (mod n), i.e.,
 *          x=A001541(a(n)) and y=A001542(a(n)) is the fundamental solution of
 *          the Pell equation x^2 - 2*(n*y)^2 = 1.
 * https://oeis.org/A298210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 8334, u32 n <= 8334, u64 n <= 8334,
 *            u128 n <= 8334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A298210
 *  a(n) = smallest k ≥ 1 such that the k‐th y–value of the Pell
 *         equation x² – 2y² = 1 satisfies y_k ≡ 0 (mod n).
 *
 *  The y–values satisfy the linear recurrence
 *        y_0 = 0,   y_1 = 2,
 *        y_k = 6·y_{k-1} – y_{k-2}   (k ≥ 2).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: all y‑values are multiples of 1. */
    if (n == 1)
        return (seq_t)1;

    /* Work modulo n.  All intermediate values stay below n,
       so 64‑bit arithmetic is safe for all n ≤ 10⁴.           */
    idx_t mod = n;                 /* modulus as native integer */
    idx_t y_prev = 0;              /* y_{k-1} */
    idx_t y_cur  = 2 % mod;        /* y_k   (k = 1) */

    /* k is the current index (starting at 1). */
    idx_t k = 1;

    /* Handle the trivial case where the first y already satisfies
       the congruence.  (This happens for n = 2, for example.)          */
    if (y_cur == 0)
        return (seq_t)k;

    /* Iterate until we find y_k ≡ 0 (mod n). */
    for (;;)
    {
        /* y_{k+1} = 6·y_k – y_{k-1}  modulo n.  Use a positive modulo
           to avoid negative intermediate results.                        */
        idx_t y_next = (6 * y_cur % mod + mod - y_prev) % mod;

        /* Advance the window. */
        y_prev = y_cur;
        y_cur  = y_next;
        ++k;

        /* Test the congruence. */
        if (y_cur == 0)
            return (seq_t)k;
    }
}
