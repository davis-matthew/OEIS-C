/*
 * A281777  Number of distinct topologies on an n-set that have exactly 9 open
 *          sets.
 * https://oeis.org/A281777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281777 – Number of distinct topologies on an n‑set
   with exactly 9 open sets.
   Formula   (OEIS): 5/6*4! Stirling2(n,4) + 5*5! Stirling2(n,5)
            + 11/2*6! Stirling2(n,6) + 3*7! Stirling2(n,7) + 8!*Stirling2(n,8)
   Implementation uses the recurrence
        S(0,0)=1, S(n,0)=0 (n>0), S(0,k)=0 (k>0)
        S(n,k)=k*S(n–1,k)+S(n–1,k–1)
   Only k = 4…8 are needed, so a small fixed‑size DP array suffices.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Quick zero for n < 4 – no partitions into ≥4 blocks. */
    if (n < 4)
        return (seq_t)0;

    /* DP arrays for Stirling numbers up to k = 8. */
    seq_t prev[9] = {0}, cur[9] = {0};
    prev[0] = (seq_t)1;            /* S(0,0) */

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = (seq_t)0;
        for (int k = 1; k <= 8; ++k) {
            /* k is small (≤8), safe to multiply directly. */
            cur[k] = (seq_t)k * prev[k] + prev[k - 1];
        }
        /* copy current row to previous for next iteration */
        for (int k = 0; k <= 8; ++k)
            prev[k] = cur[k];
    }

    /* Coefficients from the formula, all integers. */
    const seq_t c4 = (seq_t)20;    /* 5/6 * 4! */
    const seq_t c5 = (seq_t)600;   /* 5   * 5! */
    const seq_t c6 = (seq_t)3960;  /* 11/2 * 6! */
    const seq_t c7 = (seq_t)15120; /* 3   * 7! */
    const seq_t c8 = (seq_t)40320; /* 8!          */

    seq_t result = (seq_t)0;
    result += c4 * prev[4];
    result += c5 * prev[5];
    result += c6 * prev[6];
    result += c7 * prev[7];
    result += c8 * prev[8];

    return result;
}
