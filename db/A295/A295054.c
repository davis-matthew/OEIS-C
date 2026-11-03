/*
 * A295054  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(1) + b(2) + ... + b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + Σ_{i=1}^{n-1} b(i); a(0) = 1, a(1) = 2,
 *            b(0) = 3, (a(n)) and (b(n)) are increasing complementary
 *            sequences
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* b(0) = 3 and b(i) is the least integer > b(i-1) that is not an a-value;
   a grows much faster than b, so the a-values needed are always known.
   (The original grew seq_t arrays with raw realloc and searched a "used"
   list that was not sorted, so it missed some b-values.) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t len = 0, cap = 0;
    static seq_t blast = 3, S = 0;          /* b(k-1); b(1) + ... + b(k-1) */
    static idx_t j = 0;                     /* first index with A[j] >= the candidate */
    if (n + 1 > cap) {
        idx_t c = cap ? cap : 8;
        while (c < n + 1) c *= 2;
        seq_t *t = mem_resize(seq_t, A, c);
        if (!t) abort();
        A = t;
        cap = c;
    }
    if (len == 0) {
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        len = 2;
    }
    for (idx_t k = len; k <= n; ++k) {
        seq_t c = blast + (seq_t)1;         /* next b */
        for (;;) {
            while (j < len && A[j] < c) ++j;
            if (j < len && A[j] == c) c = c + (seq_t)1;
            else break;
        }
        blast = c;
        S = S + c;
        A[k] = A[k - 1] + A[k - 2] + S;
        len = k + 1;
    }
    return A[n];
}
