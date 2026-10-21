/*
 * A291381  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S^6.
 * https://oeis.org/A291381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291381 – p-INVERT of (1,1,0,0,0,0,…) with p(S)=1−S⁶
 * Recurrence (for n ≥ 13):
 *   a(n) = a(n‑6) + 6·a(n‑7) + 15·a(n‑8) + 20·a(n‑9)
 *            + 15·a(n‑10) + 6·a(n‑11) + a(n‑12)
 * Initial values:
 *   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *   a(5)=1,  a(6)=6,  a(7)=15, a(8)=20, a(9)=15,
 *   a(10)=6, a(11)=2, a(12)=12
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache – grows as needed, works in C and C++   */
    static seq_t *seq = NULL;
    static size_t cap  = 0;

    /* hard‑coded initial segment */
    static const seq_t init[13] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)6, (seq_t)15, (seq_t)20, (seq_t)15,
        (seq_t)6, (seq_t)2, (seq_t)12
    };

    /* initialise cache on first call */
    if (!seq) {
        cap = 13;
#ifdef __cplusplus
        seq = new seq_t[cap];
        for (size_t i = 0; i < cap; ++i) seq[i] = init[i];
#else
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;
        for (size_t i = 0; i < cap; ++i) seq[i] = init[i];
#endif
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i) tmp[i] = seq[i];
        delete [] seq;
        seq = tmp;
#else
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;
#endif
        cap = new_cap;
    }

    /* return already known values */
    if (n < 13)
        return seq[n];

    /* compute missing values up to n */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        seq_t v = seq[i-6]
                + (seq_t)6  * seq[i-7]
                + (seq_t)15 * seq[i-8]
                + (seq_t)20 * seq[i-9]
                + (seq_t)15 * seq[i-10]
                + (seq_t)6  * seq[i-11]
                + seq[i-12];
        seq[i] = v;
    }

    return seq[n];
}
