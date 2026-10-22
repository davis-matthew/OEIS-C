/*
 * A356520  Numbers k such that A000005(A007953(k)) = A007953(k).
 * https://oeis.org/A356520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th positive integer whose decimal digit sum equals
 *            1 or 2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Find the length D (number of digits) that contains the n-th number.
       Total numbers with at most D digits = D*(D+3)/2.  We accumulate until >= n. */
    idx_t D = 0;
    idx_t cumulative = 0;
    idx_t prev_cumulative = 0;
    while (cumulative < n) {
        prev_cumulative = cumulative;
        D++;
        cumulative += D + 1;   /* D+1 numbers of length D */
    }

    /* Offset within the block of length D (0‑based). */
    idx_t offset = n - prev_cumulative - 1;   /* 0 .. D */

    /* 10^(D-1) */
    seq_t base = pow10(D - 1);

    if (offset == 0) {
        /* 10^(D-1) – sum already 1 */
        return base;
    } else if (offset <= D - 1) {
        /* 10^(D-1) + 10^(offset-1) – a leading 1 and one other 1. */
        seq_t add = pow10(offset - 1);
        return base + add;
    } else {  /* offset == D */
        /* 2*10^(D-1) – leading digit 2 */
        return base + base;
    }
}
