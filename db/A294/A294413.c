/*
 * A294413  Solution of the complementary equation a(n) = a(n-1) + a(n-2) -
 *          b(n-1) + 6, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = a(n‑1) + a(n‑2) – b(n‑1) + 6          (n≥2)
 * a(0) = 1 , a(1) = 3
 * b(0) = 2 , b(1) = 4
 * b(n) is the smallest positive integer that has not appeared
 * in any earlier a(k) or b(k).
 *
 * The routine computes a(n) iteratively, keeping a small bitmap of
 * numbers that have already been used.  The bitmap size is
 * 2*n+10, which is always large enough for the current b(n)
 * values because at most 2*n numbers are used by step n.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* bitmap size: enough to contain all b values up to step n */
    size_t limit = (size_t)(2 * n + 10);

    /* allocate and initialise bitmap */
    unsigned char *used = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    if (!used) return (seq_t)0;                  /* fallback, should not happen */

    /* mark the initial values as used */
    used[1] = 1;      /* a(0) */
    used[2] = 1;      /* b(0) */
    used[3] = 1;      /* a(1) */
    used[4] = 1;      /* b(1) */

    int curMin = 5;                     /* candidate for next b(n) */
    seq_t aPrevPrev = (seq_t)1;   /* a(0)   */
    seq_t aPrev     = (seq_t)3;   /* a(1)   */
    seq_t lastB     = (seq_t)4;   /* b(1)   */

    for (idx_t k = 2; k <= n; ++k) {
        /* compute a(k) */
        seq_t aCur = aPrev + aPrevPrev - lastB + 6;

        /* mark aCur in the bitmap if it is small enough */
        seq_t limit_seq = (seq_t)limit;
        if (aCur <= limit_seq)
            used[(size_t)aCur] = 1;

        /* shift a values for next iteration */
        aPrevPrev = aPrev;
        aPrev     = aCur;

        /* find the next unused integer → b(k) */
        while (curMin <= (int)limit && used[curMin])
            ++curMin;
        seq_t nextB = (seq_t)curMin;
        lastB = nextB;

        if (curMin <= (int)limit)
            used[(size_t)curMin] = 1;
        ++curMin;
    }

    free(used);
    return aPrev;
}
