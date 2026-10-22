/*
 * A367346  Numbers k such that there is more than one possible solution for
 *          A367338(k).
 * https://oeis.org/A367346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367346: Numbers k such that there is more than one possible
   solution for A367338(k).  For n ≥ 5 the terms have the form
   d followed by i copies of the digit 9, then d, then (9‑d),
   where d = 1…8 and i = 0,1,2,…; the first four terms are
   14, 33, 52, 71.  The n‑th term for n ≥ 5 can be produced by
     i = (n−5)/8   (integer division)
   d = (n−5) mod 8 + 1
   a(n) = d ·10^{i+2} + 9·10^{i+1} + d·10 + (9−d)   (computationally,
   constructed digit by digit). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* leading exceptional terms */
    if (n == 1) return 14;
    if (n == 2) return 33;
    if (n == 3) return 52;
    if (n == 4) return 71;

    /* for n ≥ 5 */
    idx_t offset = n - 5;
    idx_t i = offset / 8;          /* number of 9's */
    idx_t d = offset % 8 + 1;      /* digit d (1–8) */

    seq_t val = d;                 /* first digit d */
    for (idx_t k = 0; k < i; ++k)  /* insert i copies of 9 */
        val = val * 10 + 9;
    val = val * 10 + d;            /* second d */
    val = val * 10 + (9 - d);      /* final digit (9‑d) */

    return val;
}
