/*
 * A364245  Number of parts in all partitions of 2n into parts with
 *          multiplicity <= n.
 * https://oeis.org/A364245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(2n) – s(n-1) – (n+1)·p(n-1) where p(m) is the
 *            unrestricted partition number of m, and s(m) is the sum of the
 *            numbers of parts over all partitions of m
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 50, u64 n <= 177, u128 n <= 659,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc, free, abort, calloc */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)0;

    static seq_t *P = NULL;  /* partition numbers p[0..M] */
    static seq_t *S = NULL;  /* sum of parts s[0..M] */
    static idx_t  maxComputedN = 0;   /* largest n for which P,S are up to date */

    idx_t M = 2 * n;   /* maximum sum needed for this n */

    /* Recompute tables if we haven't yet handled up to this n */
    if (n > maxComputedN) {
        /* Allocate new arrays of size M+1 (indices 0..M) */
        seq_t *newP = (seq_t *)calloc(M + 1, sizeof(seq_t));
        seq_t *newS = (seq_t *)calloc(M + 1, sizeof(seq_t));
        if (!newP || !newS) {
            /* If memory allocation fails, abort (should not happen in tests) */
            abort();
        }

        /* Base case: p(0) = 1, s(0) = 0 */
        newP[0] = (seq_t)1;
        newS[0] = (seq_t)0;

        /* Compute partition numbers and sums up to M */
        for (idx_t i = 1; i <= M; ++i) {
            for (idx_t s = i; s <= M; ++s) {
                /* new partitions formed by adding part i */
                newP[s] += newP[s - i];
                /* each such partition gets one more part */
                newS[s] += newS[s - i] + newP[s - i];
            }
        }

        /* Free old arrays and replace them */
        free(P);
        free(S);
        P = newP;
        S = newS;
        maxComputedN = n;
    }

    /* Apply the formula a(n) = s(2n) – s(n-1) – (n+1)·p(n-1) */
    seq_t result = S[2 * n];

    seq_t t1 = S[n - 1];                     /* s(n-1) */
    seq_t t2 = P[n - 1] * (seq_t)(n + 1);    /* (n+1)·p(n-1) */

    result -= t1;
    result -= t2;

    return result;
}
