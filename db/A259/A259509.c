/*
 * A259509  Number of (n+1) X (2+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0001 0101 0111.
 * https://oeis.org/A259509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+11*a(n-2)-4*a(n-3)-18*a(n-4); Initial values:
 *            a(1)=31, a(2)=129, a(3)=533, a(4)=2219
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the n-th term of OEIS sequence A259509 (offset 1).
   It uses the given linear recurrence; all calculations are done in
   seq_t, so that the code works for 8/16/32/64‑bit unsigned or any
   arbitrary‑precision integer class.  For n<1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but keep well‑defined anyway */
        return 0;

    /* Handle the first four values directly */
    if (n <= 4) {
        static const seq_t init[4] = {31, 129, 533, 2219};
        return init[n - 1];
    }

    /* Compute iteratively using the recurrence; keep only the last four
       values to save memory. */
    seq_t a1 = 31;   /* a(n-4) */
    seq_t a2 = 129;  /* a(n-3) */
    seq_t a3 = 533;  /* a(n-2) */
    seq_t a4 = 2219; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 2 * a4 + 11 * a3 - 4 * a2 - 18 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
